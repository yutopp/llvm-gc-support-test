;;
@str.1 = private unnamed_addr constant [22 x i8] c"How is your progress?\00"
@str.2 = private unnamed_addr constant [11 x i8] c"Doudesuka?\00"

;; declaration GC related functions
declare void @llvm.gcroot(i8** %ptrloc, i8* %metadata)

;; Called from entry.cpp
define void @f() gc "shadow-stack" {
entry:
    ;; allocate stack to hold pointer value for object
    ;; "pointer" to "i32*".(means i32**)
    %val_ptr.1 = alloca i32*
    %val_ptr.2 = alloca i32*

    ;; cast ptr* to i8 ptr*
    %tmp.1 = bitcast i32** %val_ptr.1 to i8**
    call void @llvm.gcroot( i8** %tmp.1, i8* getelementptr inbounds ( [22 x i8]* @str.1, i32 0, i32 0 ) )

    %tmp.2 = bitcast i32** %val_ptr.2 to i8**
    call void @llvm.gcroot( i8** %tmp.2, i8* getelementptr inbounds ( [11 x i8]* @str.2, i32 0, i32 0 ) )

    ret void
}

