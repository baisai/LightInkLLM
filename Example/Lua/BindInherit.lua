

local CppClassInherit = CppClassList.CppClassInherit


local cppObj = CppClassInherit()
cppObj:set_base(100)
print("CppClassInherit:get_base() == 100 ?", cppObj:get_base() == 100)
cppObj:set_inherit(1000)
print("CppClassInherit:get_inherit() == 1000 ?", cppObj:get_inherit() == 1000)

cppObj:test()

CppClassInherit.test_static()
CppClassInherit.test_static_base()
CppClassInherit.test_static_inherit()

cppObj.m_test = 10000
print("CppClassInherit.m_test == 100000 ? ", cppObj.m_test == 100000)

cppObj:force_destroy__()
cppObj = nil
