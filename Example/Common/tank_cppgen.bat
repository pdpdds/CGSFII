set DST_DIR=../tankserver
set SRC_DIR=.
protoc -I=%SRC_DIR% --cpp_out=%DST_DIR% %SRC_DIR%/Tank.proto
pause