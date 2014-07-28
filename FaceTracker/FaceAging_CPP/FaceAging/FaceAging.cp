/*
 *  FaceAging.cp
 *  FaceAging
 *
 *  Created by yangboz on 14-7-9.
 *  Copyright (c) 2014年 GODPAPER. All rights reserved.
 *
 */

#include <iostream>
#include "FaceAging.h"
#include "FaceAgingPriv.h"

void FaceAging::HelloWorld(const char * s)
{
	 FaceAgingPriv *theObj = new FaceAgingPriv;
	 theObj->HelloWorldPriv(s);
	 delete theObj;
};

void FaceAgingPriv::HelloWorldPriv(const char * s) 
{
	std::cout << s << std::endl;
};

