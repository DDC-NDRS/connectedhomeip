/*
 *   Copyright (c) 2022 Project CHIP Authors
 *   All rights reserved.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *
 */

#include "DeviceControllerDelegateBridge.h"
#import <Matter/Matter.h>

@implementation CHIPToolDeviceControllerDelegate
- (void)controller:(MTRDeviceController *)controller statusUpdate:(MTRCommissioningStatus)status
{
    NSLog(@"Pairing Status Update: %ld", static_cast<long>(status));
    switch (status) {
    case MTRCommissioningStatusSuccess:
        ChipLogProgress(chipTool, "Secure Pairing Success");
        ChipLogProgress(chipTool, "CASE establishment successful");
        break;
    case MTRCommissioningStatusFailed:
        ChipLogError(chipTool, "Secure Pairing Failed");
        _commandBridge->SetCommandExitStatus(CHIP_ERROR_INCORRECT_STATE);
        break;
    case MTRCommissioningStatusDiscoveringMoreDevices:
        ChipLogError(chipTool, "MTRCommissioningStatusDiscoveringMoreDevices: This should not happen.");
        break;
    case MTRCommissioningStatusUnknown:
        ChipLogError(chipTool, "Unknown Pairing Status");
        break;
    }
}

- (void)controller:(MTRDeviceController *)controller commissioningSessionEstablishmentDone:(NSError *)error
{
    if (error != nil) {
        ChipLogProgress(chipTool, "PASE establishment failed");
        _commandBridge->SetCommandExitStatus(error);
        return;
    }
    ChipLogProgress(chipTool, "Pairing Success");
    ChipLogProgress(chipTool, "PASE establishment successful");
    if (_params == nil) {
        _commandBridge->SetCommandExitStatus(nil);
        return;
    }

    NSError * commissionError;
    [_commissioner commissionNodeWithID:@(_deviceID) commissioningParams:_params error:&commissionError];
    if (commissionError != nil) {
        _commandBridge->SetCommandExitStatus(commissionError);
        return;
    }
}

- (void)controller:(MTRDeviceController *)controller commissioningComplete:(NSError *)error
{
    _commandBridge->SetCommandExitStatus(error, "Pairing Commissioning Complete");
}

- (void)controller:(MTRDeviceController *)controller commissioningComplete:(NSError *)error nodeID:(NSNumber *)nodeID metrics:(MTRMetrics *)metrics
{
    (void) nodeID;
    NSString * message = [NSString stringWithFormat:@"Pairing Commissioning Complete with metrics %@", metrics];
    _commandBridge->SetCommandExitStatus(error, [message UTF8String]);
}

- (void)controller:(MTRDeviceController *)controller commissioneeHasReceivedNetworkCredentials:(NSNumber *)nodeID
{
    NSLog(@"Node %@ has received network credentials", nodeID);
}

@end
