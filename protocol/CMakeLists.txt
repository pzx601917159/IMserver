SET(LIBPROTOCOL_SRC reguserreq.pb.cc reguserres.pb.cc loginreq.pb.cc loginres.pb.cc sendmsg.pb.cc regserverreq.pb.cc regserverres.pb.cc offline.pb.cc online.pb.cc logoutreq.pb.cc)
ADD_LIBRARY(protocol STATIC ${LIBPROTOCOL_SRC})
