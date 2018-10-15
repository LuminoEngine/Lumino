//
//  main.m
//  LuminoApp.macOS
//
//  Created by lriki on 2018/09/02.
//  Copyright © 2018年 Lumino. All rights reserved.
//

#include <LuminoEngine.hpp>
#include <LuminoEngine/Platform/CocoaPlatformInterface.hpp>

int main(int argc, const char * argv[]) {
	return ln::CocoaPlatformInterface::Main();
}
