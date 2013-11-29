// ==
// refer to
// http://llvm.org/docs/GarbageCollection.html#in-your-runtime
// http://llvm.org/docs/doxygen/html/ShadowStackGC_8cpp_source.html
// ==

#include <cstdint>


/// @brief The map for a single function's stack frame.  One of these is
///        compiled as constant data into the executable for each function.
///
/// Storage of metadata values is elided if the %metadata parameter to
/// @llvm.gcroot is null.
struct FrameMap
{
    int32_t NumRoots; // Number of roots in stack frame.
    int32_t NumMeta;  // Number of metadata descriptors. May be < NumRoots.
    void *Meta[];     // May be absent for roots without metadata.
};


/// @brief A link in the dynamic shadow stack.  One of these is embedded in
///        the stack frame of each function on the call stack.
struct StackEntry
{
    StackEntry *Next; // Caller's stack entry.
    FrameMap *Map;          // Pointer to constant FrameMap.
    void *Roots[];          // Stack roots (in-place array, so we pretend).
};


/// @brief The head of the singly-linked list of StackEntries.  Functions push
///        and pop onto this in their prologue and epilogue.
///
/// Since there is only a global list, this technique is not threadsafe.
StackEntry *llvm_gc_root_chain;


/// @brief Calls Visitor(root, meta) for each GC root on the stack.
///        root and meta are exactly the values passed to
///        @llvm.gcroot.
///
/// Visitor could be a function to recursively mark live objects.  Or it
/// might copy them to another heap or generation.
///
/// @param Visitor A function to invoke for every GC root on the stack.
// void visitGCRoots(void (*Visitor)(void **Root, const void *Meta)) {
template<typename F>
void visitGCRoots( F const& Visitor ) {
    for (StackEntry *entry = llvm_gc_root_chain; entry != nullptr; entry = entry->Next) {
        unsigned i = 0;

        // For roots [0, NumMeta), the metadata pointer is in the FrameMap.
        for (; i < entry->Map->NumMeta; ++i)
            Visitor(&entry->Roots[i], entry->Map->Meta[i]);

        // For roots [NumMeta, NumRoots), the metadata pointer is null.
        for (; i < entry->Map->NumRoots; ++i)
            Visitor(&entry->Roots[i], nullptr);
    }
}
