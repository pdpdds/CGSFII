set DST_DIR=../protobufserver
set SRC_DIR=.
protoc -I=%SRC_DIR% --cpp_out=%DST_DIR% %SRC_DIR%/samplemessage.proto
pause