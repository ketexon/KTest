add_executable(
	KTestUnit
	Factorial.cpp
	CustomType.cpp
	Printing.cpp
	Approx.cpp
)

target_link_libraries(
	KTestUnit
	PRIVATE
	KTestWithMain
)

set_target_properties(
	KTestUnit
	PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)

target_compile_options(
	KTestUnit
	PRIVATE
	-Od
)