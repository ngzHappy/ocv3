TEMPLATE = subdirs

#.subdir
#Use the specified subdirectory instead of SUBDIRS value.
#.file
#Specify the subproject pro file explicitly. Cannot be used in conjunction with .subdir modifier.
#.depends
#This subproject depends on specified subproject.
#.makefile
#The makefile of subproject. Available only on platforms that use makefiles.
#.target
#Base string used for makefile targets related to this subproject. Available only on platforms that use makefiles.

lib_cplusplus_base_library.file = $$PWD/cplusplus_base_library/cplusplus_base_library.pro
lib_qcustomplot_library.file = $$PWD/qcustomplot_library/QCustomPlot.pro

app_test_emperty.file = $$PWD/test/emperty/emperty.pro
app_test_emperty.depends+=lib_cplusplus_base_library
app_test_emperty.depends+=lib_qcustomplot_library

app_test_lua.file = $$PWD/test/test_lua/test_lua.pro
app_test_lua.depends+=lib_cplusplus_base_library
app_test_lua.depends+=lib_qcustomplot_library

app_test_function.file = $$PWD/test/test_function/test_function.pro
app_test_function.depends+=lib_cplusplus_base_library
app_test_function.depends+=lib_qcustomplot_library

#test_textedit
app_test_textedit.file = $$PWD/test/test_textedit/test_textedit.pro
app_test_textedit.depends+=lib_cplusplus_base_library
app_test_textedit.depends+=lib_qcustomplot_library

SUBDIRS += \
    lib_cplusplus_base_library \
    lib_qcustomplot_library \
    app_test_emperty \
    app_test_lua \
    app_test_function \
    app_test_textedit















