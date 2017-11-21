

local CppClassAbsInherit = CppClassList.CppClassAbsInherit


local cppObj = CppClassAbsInherit.get_shared()

CppClassAbsInherit.check_shared(cppObj)

cppObj:test_abstract()

cppObj:force_delete__()
cppObj = nil
