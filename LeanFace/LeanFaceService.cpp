//
//  LeanFaceService.cpp
//  LeanFace
//
//  Created by yangboz on 10/10/15.
//  Copyright (c) 2015 SMARTKIT.INFO. All rights reserved.
//

#include "Thrift/gen-cpp/LeanFaceService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PosixThreadFactory.h>
#include <thrift/server/TNonblockingServer.h>
#include <thrift/server/TThreadPoolServer.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;

#include <stdio.h>

int main(int argc, char** argv)
{
    return 0;
}