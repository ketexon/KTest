add_executable(
	KTestUnit
	Factorial.cpp
	CustomType.cpp
)

target_link_libraries(
	KTestUnit
	PRIVATE
	KTestWithMain
)

set_target_properties(
	KTestUnit
	PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)