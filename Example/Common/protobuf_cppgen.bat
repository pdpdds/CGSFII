set DST_DIR=../example/protobufserver
set SRC_DIR=.
protoc -I=%SRC_DIR% --cpp_out=%DST_DIR% %SRC_DIR%/samplemessage.proto
pause