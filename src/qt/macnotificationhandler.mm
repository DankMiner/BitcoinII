// Copyright (c) 2009-2025 Satoshi Nakamoto
// Copyright (c) 2009-2024 The Bitcoin Core developers
// Copyright (c) 2025 The BitcoinII developers
// Forked from Bitcoin Core version 0.27.0
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#include "macnotificationhandler.h"

#undef slots
#import <objc/runtime.h>
#include <Cocoa/Cocoa.h>

// Add an obj-c category (extension) to return the expected bundle identifier
@implementation NSBundle(returnCorrectIdentifier)
- (NSString *)__bundleIdentifier
{
    if (self == [NSBundle mainBundle]) {
        return @"org.bitcoinIIfoundation.BitcoinII-Qt";
    } else {
        return [self __bundleIdentifier];
    }
}
@end

void MacNotificationHandler::showNotification(const QString &title, const QString &text)
{
    // check if users OS has support for NSUserNotification
    if(this->hasUserNotificationCenterSupport()) {
        NSUserNotification* userNotification = [[NSUserNotification alloc] init];
        userNotification.title = title.toNSString();
        userNotification.informativeText = text.toNSString();
        [[NSUserNotificationCenter defaultUserNotificationCenter] deliverNotification: userNotification];
        [userNotification release];
    }
}

bool MacNotificationHandler::hasUserNotificationCenterSupport(void)
{
    Class possibleClass = NSClassFromString(@"NSUserNotificationCenter");

    // check if users OS has support for NSUserNotification
    if(possibleClass!=nil) {
        return true;
    }
    return false;
}


MacNotificationHandler *MacNotificationHandler::instance()
{
    static MacNotificationHandler *s_instance = nullptr;
    if (!s_instance) {
        s_instance = new MacNotificationHandler();

        Class aPossibleClass = objc_getClass("NSBundle");
        if (aPossibleClass) {
            // change NSBundle -bundleIdentifier method to return a correct bundle identifier
            // a bundle identifier is required to use OSXs User Notification Center
            method_exchangeImplementations(class_getInstanceMethod(aPossibleClass, @selector(bundleIdentifier)),
                                           class_getInstanceMethod(aPossibleClass, @selector(__bundleIdentifier)));
        }
    }
    return s_instance;
}
