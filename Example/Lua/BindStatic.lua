


test_static()

print("test_cclosure return", 
	test_cclosure({name = "chendong", email = "baisaichen@live.com"}))

print("int_static == ", int_static)
print("int_static == ", get_userdata_property(int_static))
set_userdata_property(int_static, 100)
print("int_static == ", get_userdata_property(int_static))


print("this is print()")
print_debug("this is print_debug()")
print_warning("this is print_warning")
print_error("this is print_error")