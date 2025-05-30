/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
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

/**
 *    @file
 *          Example project configuration file for CHIP.
 *
 *          This is a place to put application or project-specific overrides
 *          to the default configuration values for general CHIP features.
 *
 */

#pragma once

#define CHIP_CONFIG_LOG_MODULE_Zcl_PROGRESS 0
#define CHIP_CONFIG_LOG_MODULE_InteractionModel_PROGRESS 0
#define CHIP_CONFIG_LOG_MODULE_InteractionModel_DETAIL 0
#define CHIP_CONFIG_LOG_MODULE_DataManagement_PROGRESS 0
#define CHIP_CONFIG_LOG_MODULE_FabricProvisioning_PROGRESS 0
#define CHIP_CONFIG_LOG_MODULE_SecureChannel_PROGRESS 0

// Enable `Extension` attribute of ACL Cluster as required by door locks
#define CHIP_CONFIG_ENABLE_ACL_EXTENSIONS 1
