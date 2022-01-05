/*
 * Copyright (c) 2006-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"
#include <ios>
#include <iostream>
#include <iomanip>
#include "display.h"
#include "sensors.h"

// This is my version of main.cpp
// this is my second comment, this is the initial fork
// hello i am a comment

Thread sendingThreadHandle;
Thread displayThreadHandle;

int main(void)
{
    sendingThreadHandle.start(callback(sendThread));
    displayThreadHandle.start(callback(displayThread));
}
