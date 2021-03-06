;; int32_t* val_x;
;; for( unsigned int i=0; i<20; ++i ) {
;;     val_x = my_alloc();
;;     *val_x = i;
;;     put_int32( val_x );
;; }

;; Called from entry.cpp
define void @f() gc "shadow-stack" {
entry:
    ;; allocate stack to hold pointer value for object
    ;; so val_x_ptr is "pointer" to "i32*".(means i32**)
    %val_x_ptr = alloca i32*

    ;; cast ptr* to i8 ptr*
    %tmp = bitcast i32** %val_x_ptr to i8**
    call void @llvm.gcroot(i8** %tmp, i8* null)
    br label %loop.head

loop.head:
    %i = phi i32 [ 0, %entry ], [ %next_i, %loop.body ]

    %loopcond = icmp ult i32 %i, 20
    br i1 %loopcond, label %loop.body, label %loop.end

loop.body:
    %next_i = add i32 %i, 1

    call void @put_line()

    ;; allocate memory
    %allocated_memory_ptr = call i32* @my_alloc()
    store i32* %allocated_memory_ptr, i32** %val_x_ptr
 
    ;; set i to the space pointed i32*
    store i32 %i, i32* %allocated_memory_ptr

    ;; show
    call void @put_int32( i32* %allocated_memory_ptr )

    br label %loop.head

loop.end:
    ret void
}


;; declaration of GC related functions
declare void @llvm.gcroot(i8** %ptrloc, i8* %metadata)

;; declaration of runtime functions
;; defined in oreore_runtime.cpp
declare i32* @my_alloc()
declare void @put_int32(i32* %ptr)
declare void @put_line()