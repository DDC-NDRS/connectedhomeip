/*
 *
 *    Copyright (c) 2019 Google LLC.
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#pragma once
#include <app/clusters/door-lock-server/door-lock-server.h>

#include <stdbool.h>
#include <stdint.h>

#include "AppEvent.h"
#include "CHIPProjectConfig.h"

#include <cmsis_os2.h>
#include <lib/core/CHIPError.h>

#include <lib/support/DefaultStorageKeyAllocator.h>

namespace SilabsDoorLock {

namespace LockInitParams {

struct LockParam
{
    // Read from zap attributes
    uint16_t numberOfUsers                  = 0;
    uint8_t numberOfCredentialsPerUser      = 0;
    uint8_t numberOfWeekdaySchedulesPerUser = 0;
    uint8_t numberOfYeardaySchedulesPerUser = 0;
    uint8_t numberOfHolidaySchedules        = 0;
};

class ParamBuilder
{
public:
    ParamBuilder & SetNumberOfUsers(uint16_t numberOfUsers)
    {
        lockParam_.numberOfUsers = numberOfUsers;
        return *this;
    }
    ParamBuilder & SetNumberOfCredentialsPerUser(uint8_t numberOfCredentialsPerUser)
    {
        lockParam_.numberOfCredentialsPerUser = numberOfCredentialsPerUser;
        return *this;
    }
    ParamBuilder & SetNumberOfWeekdaySchedulesPerUser(uint8_t numberOfWeekdaySchedulesPerUser)
    {
        lockParam_.numberOfWeekdaySchedulesPerUser = numberOfWeekdaySchedulesPerUser;
        return *this;
    }
    ParamBuilder & SetNumberOfYeardaySchedulesPerUser(uint8_t numberOfYeardaySchedulesPerUser)
    {
        lockParam_.numberOfYeardaySchedulesPerUser = numberOfYeardaySchedulesPerUser;
        return *this;
    }
    ParamBuilder & SetNumberOfHolidaySchedules(uint8_t numberOfHolidaySchedules)
    {
        lockParam_.numberOfHolidaySchedules = numberOfHolidaySchedules;
        return *this;
    }
    LockParam GetLockParam() { return lockParam_; }

private:
    LockParam lockParam_;
};

} // namespace LockInitParams

namespace Storage {

using namespace SilabsDoorLockConfig::ResourceRanges;
struct WeekDayScheduleInfo
{
    DlScheduleStatus status;
    EmberAfPluginDoorLockWeekDaySchedule schedule;
};

struct YearDayScheduleInfo
{
    DlScheduleStatus status;
    EmberAfPluginDoorLockYearDaySchedule schedule;
};

struct HolidayScheduleInfo
{
    DlScheduleStatus status;
    EmberAfPluginDoorLockHolidaySchedule schedule;
};

struct LockUserInfo
{
    char userName[DOOR_LOCK_MAX_USER_NAME_SIZE];
    size_t userNameSize;
    uint32_t userUniqueId;
    UserStatusEnum userStatus;
    UserTypeEnum userType;
    CredentialRuleEnum credentialRule;
    chip::EndpointId endpointId;
    chip::FabricIndex createdBy;
    chip::FabricIndex lastModifiedBy;
    uint16_t currentCredentialCount;
};

struct LockCredentialInfo
{
    DlCredentialStatus status;
    CredentialTypeEnum credentialType;
    chip::FabricIndex createdBy;
    chip::FabricIndex lastModifiedBy;
    uint8_t credentialData[kMaxCredentialSize];
    size_t credentialDataSize;
};
} // namespace Storage
} // namespace SilabsDoorLock

using namespace ::chip;
using namespace SilabsDoorLockConfig::ResourceRanges;
using namespace SilabsDoorLock::Storage;

class LockManager
{
public:
    enum Action_t
    {
        LOCK_ACTION = 0,
        UNLOCK_ACTION,
        UNLATCH_ACTION,

        INVALID_ACTION
    } Action;

    enum State_t
    {
        kState_LockInitiated = 0,
        kState_LockCompleted,
        kState_UnlockInitiated,
        kState_UnlatchInitiated,
        kState_UnlockCompleted,
        kState_UnlatchCompleted,
    } State;

    CHIP_ERROR Init(chip::app::DataModel::Nullable<chip::app::Clusters::DoorLock::DlLockState> state,
                    SilabsDoorLock::LockInitParams::LockParam lockParam, PersistentStorageDelegate * storage);
    bool NextState();
    bool IsActionInProgress();
    bool InitiateAction(int32_t aActor, Action_t aAction);

    typedef void (*Callback_fn_initiated)(Action_t, int32_t aActor);
    typedef void (*Callback_fn_completed)(Action_t);
    void SetCallbacks(Callback_fn_initiated aActionInitiated_CB, Callback_fn_completed aActionCompleted_CB);

    bool Lock(chip::EndpointId endpointId, const Nullable<chip::FabricIndex> & fabricIdx, const Nullable<chip::NodeId> & nodeId,
              const Optional<chip::ByteSpan> & pin, OperationErrorEnum & err);
    bool Unlock(chip::EndpointId endpointId, const Nullable<chip::FabricIndex> & fabricIdx, const Nullable<chip::NodeId> & nodeId,
                const Optional<chip::ByteSpan> & pin, OperationErrorEnum & err);
    bool Unbolt(chip::EndpointId endpointId, const Nullable<chip::FabricIndex> & fabricIdx, const Nullable<chip::NodeId> & nodeId,
                const Optional<chip::ByteSpan> & pin, OperationErrorEnum & err);

    bool GetUser(chip::EndpointId endpointId, uint16_t userIndex, EmberAfPluginDoorLockUserInfo & user);
    bool SetUser(chip::EndpointId endpointId, uint16_t userIndex, chip::FabricIndex creator, chip::FabricIndex modifier,
                 const chip::CharSpan & userName, uint32_t uniqueId, UserStatusEnum userStatus, UserTypeEnum usertype,
                 CredentialRuleEnum credentialRule, const CredentialStruct * credentials, size_t totalCredentials);

    bool GetCredential(chip::EndpointId endpointId, uint16_t credentialIndex, CredentialTypeEnum credentialType,
                       EmberAfPluginDoorLockCredentialInfo & credential);

    bool SetCredential(chip::EndpointId endpointId, uint16_t credentialIndex, chip::FabricIndex creator, chip::FabricIndex modifier,
                       DlCredentialStatus credentialStatus, CredentialTypeEnum credentialType,
                       const chip::ByteSpan & credentialData);

    DlStatus GetWeekdaySchedule(chip::EndpointId endpointId, uint8_t weekdayIndex, uint16_t userIndex,
                                EmberAfPluginDoorLockWeekDaySchedule & schedule);

    DlStatus SetWeekdaySchedule(chip::EndpointId endpointId, uint8_t weekdayIndex, uint16_t userIndex, DlScheduleStatus status,
                                DaysMaskMap daysMask, uint8_t startHour, uint8_t startMinute, uint8_t endHour, uint8_t endMinute);

    DlStatus GetYeardaySchedule(chip::EndpointId endpointId, uint8_t yearDayIndex, uint16_t userIndex,
                                EmberAfPluginDoorLockYearDaySchedule & schedule);

    DlStatus SetYeardaySchedule(chip::EndpointId endpointId, uint8_t yearDayIndex, uint16_t userIndex, DlScheduleStatus status,
                                uint32_t localStartTime, uint32_t localEndTime);

    DlStatus GetHolidaySchedule(chip::EndpointId endpointId, uint8_t holidayIndex, EmberAfPluginDoorLockHolidaySchedule & schedule);

    DlStatus SetHolidaySchedule(chip::EndpointId endpointId, uint8_t holidayIndex, DlScheduleStatus status, uint32_t localStartTime,
                                uint32_t localEndTime, OperatingModeEnum operatingMode);

    bool IsValidUserIndex(uint16_t userIndex);
    bool IsValidCredentialIndex(uint16_t credentialIndex, CredentialTypeEnum type);
    bool IsValidCredentialType(CredentialTypeEnum type);
    bool IsValidWeekdayScheduleIndex(uint8_t scheduleIndex);
    bool IsValidYeardayScheduleIndex(uint8_t scheduleIndex);
    bool IsValidHolidayScheduleIndex(uint8_t scheduleIndex);

    bool setLockState(chip::EndpointId endpointId, const Nullable<chip::FabricIndex> & fabricIdx,
                      const Nullable<chip::NodeId> & nodeId, DlLockState lockState, const Optional<chip::ByteSpan> & pin,
                      OperationErrorEnum & err);
    const char * lockStateToString(DlLockState lockState) const;

    void UnlockAfterUnlatch();

private:
    struct UnlatchContext
    {
        static constexpr uint8_t kMaxPinLength = UINT8_MAX;
        uint8_t mPinBuffer[kMaxPinLength];
        uint8_t mPinLength;
        chip::EndpointId mEndpointId;
        Nullable<chip::FabricIndex> mFabricIdx;
        Nullable<chip::NodeId> mNodeId;
        OperationErrorEnum mErr;

        void Update(chip::EndpointId endpointId, const Nullable<chip::FabricIndex> & fabricIdx,
                    const Nullable<chip::NodeId> & nodeId, const Optional<chip::ByteSpan> & pin, OperationErrorEnum & err)
        {
            mEndpointId = endpointId;
            mFabricIdx  = fabricIdx;
            mNodeId     = nodeId;
            mErr        = err;

            if (pin.HasValue())
            {
                memcpy(mPinBuffer, pin.Value().data(), pin.Value().size());
                mPinLength = static_cast<uint8_t>(pin.Value().size());
            }
            else
            {
                memset(mPinBuffer, 0, kMaxPinLength);
                mPinLength = 0;
            }
        }
    };
    UnlatchContext mUnlatchContext;
    chip::EndpointId mCurrentEndpointId;

    friend LockManager & LockMgr();
    State_t mState;

    Callback_fn_initiated mActionInitiated_CB;
    Callback_fn_completed mActionCompleted_CB;

    void CancelTimer(void);
    void StartTimer(uint32_t aTimeoutMs);

    static void TimerEventHandler(void * timerCbArg);
    static void AutoLockTimerEventHandler(AppEvent * aEvent);
    static void ActuatorMovementTimerEventHandler(AppEvent * aEvent);

    osTimerId_t mLockTimer;

    SilabsDoorLock::LockInitParams::LockParam LockParams;

    // Stores LockUserInfo corresponding to a user index
    static StorageKeyName LockUserEndpoint(chip::EndpointId endpoint, uint16_t userIndex)
    {
        return StorageKeyName::Formatted("g/e/%x/lu/%x", endpoint, userIndex);
    }
    // Stores LockCredentialInfo corresponding to a credential index and type
    static StorageKeyName LockCredentialEndpoint(chip::EndpointId endpoint, CredentialTypeEnum credentialType,
                                                 uint16_t credentialIndex)
    {
        return StorageKeyName::Formatted("g/e/%x/ct/%x/lc/%x", endpoint, to_underlying(credentialType), credentialIndex);
    }
    // Stores all the credential indices that belong to a user
    static StorageKeyName LockUserCredentialMap(uint16_t userIndex) { return StorageKeyName::Formatted("g/lu/%x/lc", userIndex); }
    // Stores WeekDayScheduleInfo corresponding to a user and schedule index
    static StorageKeyName LockUserWeekDayScheduleEndpoint(chip::EndpointId endpoint, uint16_t userIndex, uint16_t scheduleIndex)
    {
        return StorageKeyName::Formatted("g/e/%x/lu/%x/lw/%x", endpoint, userIndex, scheduleIndex);
    }
    // Stores YearDayScheduleInfo corresponding to a user and schedule index
    static StorageKeyName LockUserYearDayScheduleEndpoint(chip::EndpointId endpoint, uint16_t userIndex, uint16_t scheduleIndex)
    {
        return StorageKeyName::Formatted("g/e/%x/lu/%x/ly/%x", endpoint, userIndex, scheduleIndex);
    }
    // Stores HolidayScheduleInfo corresponding to a schedule index
    static StorageKeyName LockHolidayScheduleEndpoint(chip::EndpointId endpoint, uint16_t scheduleIndex)
    {
        return StorageKeyName::Formatted("g/e/%x/lh/%x", endpoint, scheduleIndex);
    }

    bool MigrateConfig(const SilabsDoorLock::LockInitParams::LockParam & params);

    // Pointer to the PeristentStorage
    PersistentStorageDelegate * mStorage = nullptr;
};

LockManager & LockMgr();
