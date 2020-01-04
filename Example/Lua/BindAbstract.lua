

local CppClassAbsInherit = CppClassList.CppClassAbsInherit


local cppObj = CppClassAbsInherit.get_shared()

CppClassAbsInherit.check_shared(cppObj)

cppObj:test_abstract()

CppClassAbsInherit.show_class_name(cppObj)

cppObj:force_destroy__()
cppObj = nil
