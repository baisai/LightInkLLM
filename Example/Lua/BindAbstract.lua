

local CppClassAbsInherit = CppClassList.CppClassAbsInherit


local cppObj = CppClassAbsInherit.get_shared()

cppObj:test_abstract()

cppObj:force_delete__()
cppObj = nil
