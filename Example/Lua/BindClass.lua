

local CppClass = CppClassList.CppClass

local cppObj = CppClass:new__(1)
print("cppObj:get_private() == 1 ? ", cppObj:get_private() == 1)
cppObj:set_private(100)
print("cppObj:get_private() == 100 ? ", cppObj:get_private() == 100)
cppObj:delete__()
cppObj = nil

CppClass.test_static()

cppObj = CppClass(2) --auto gc
cppObj.m_char = 1
cppObj.m_short = 2
cppObj.m_int = 3
cppObj.m_string = "chendong"
local t = {}
for k = 1, 10 do
	t[k] = k
end
cppObj.m_vector = t
print("cppObj.m_char == 1 ? ", cppObj.m_char == 1)
print("cppObj.m_short == 2 ? ", cppObj.m_short == 2)
print("cppObj.m_int == 3 ? ", cppObj.m_int == 3)
print("cppObj.m_string == \"chendong\" ? ", cppObj.m_string == "chendong")
print("cppObj.m_vector == {1,2,3,4,5,6,7,8,9,10} ? ", unpack(cppObj.m_vector))
cppObj.test_static()
cppObj = nil
collectgarbage("collect")



local CppClass2 = CppClassList.CppClass2
cppObj = CppClass2.create()
cppObj:test()
CppClass2.destroy(cppObj)
cppObj = nil

cppObj = CppClass2:new__() --this will be error!!! CppClass2 disable new