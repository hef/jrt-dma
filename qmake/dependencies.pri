# On windows, a shared object is a .dll
win32: SONAME=dll
else:  SONAME=so

# This function sets up the dependencies for libraries that are built with
# this project.  Specify the libraries you need to depend on in the variable
# DEPENDENCY_LIBRARIES and this will add
for(dep, DEPENDENCY_LIBRARIES) {
    message($$TARGET depends on $$dep ($${DESTDIR}/$${dep}.$${SONAME}))
    LIBS += $${DESTDIR}/lib$${dep}.$${SONAME}
    PRE_TARGETDEPS += $${DESTDIR}/lib$${dep}.$${SONAME}
}

# reference: http://stackoverflow.com/questions/2288292/qmake-project-dependencies-linked-libraries
