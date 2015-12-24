SET(LIBMYSQLC_SRC db_pool.cpp mysqldatabase.cpp readconfig.cpp)
ADD_LIBRARY(mysqlc STATIC ${LIBMYSQLC_SRC})
