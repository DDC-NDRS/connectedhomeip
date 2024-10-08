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
package com.chip.casting;

import com.matter.casting.core.CastingPlayer;
import com.matter.casting.core.Endpoint;
import java.net.InetAddress;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

/** @deprecated Use the APIs described in /examples/tv-casting-app/APIs.md instead. */
@Deprecated
public class VideoPlayer {
  private static final String TAG = VideoPlayer.class.getSimpleName();

  private CastingPlayer castingPlayer;
  private long nodeId;
  private byte fabricIndex;
  private String deviceName;
  private int vendorId;
  private int productId;
  private int deviceType;
  private List<ContentApp> contentApps;
  private boolean isConnected = false;

  private int numIPs;
  private List<InetAddress> ipAddresses;
  private String hostName;
  private String instanceName;
  private long lastDiscoveredMs;
  private String MACAddress;
  private boolean isAsleep = false;
  private int port;

  private boolean isInitialized = false;

  public VideoPlayer(
      long nodeId,
      byte fabricIndex,
      String deviceName,
      int vendorId,
      int productId,
      int deviceType,
      List<ContentApp> contentApps,
      int numIPs,
      List<InetAddress> ipAddresses,
      String hostName,
      String instanceName,
      int port,
      long lastDiscoveredMs,
      String MACAddress,
      boolean isAsleep,
      boolean isConnected) {
    this.nodeId = nodeId;
    this.fabricIndex = fabricIndex;
    this.deviceName = deviceName;
    this.vendorId = vendorId;
    this.productId = productId;
    this.deviceType = deviceType;
    this.contentApps = contentApps;
    this.isConnected = isConnected;
    this.numIPs = numIPs;
    this.ipAddresses = ipAddresses;
    this.hostName = hostName;
    this.MACAddress = MACAddress;
    this.lastDiscoveredMs = lastDiscoveredMs;
    this.instanceName = instanceName;
    this.port = port;
    this.isAsleep = isAsleep;
    this.isInitialized = true;
  }

  public VideoPlayer(CastingPlayer castingPlayer) {
    this.castingPlayer = castingPlayer;
    this.deviceType = (int) castingPlayer.getDeviceType();
    this.deviceName = castingPlayer.getDeviceName();
    this.hostName = castingPlayer.getHostName();
    this.ipAddresses = castingPlayer.getIpAddresses();
    this.numIPs =
        castingPlayer.getIpAddresses() != null ? castingPlayer.getIpAddresses().size() : 0;
    this.isConnected = castingPlayer.isConnected();
    this.productId = castingPlayer.getProductId();
    this.vendorId = castingPlayer.getVendorId();
    List<Endpoint> endpoints = castingPlayer.getEndpoints();
    if (endpoints != null) {
      this.contentApps = new ArrayList<>();
      for (Endpoint endpoint : endpoints) {
        contentApps.add(new ContentApp(endpoint));
      }
    }
    this.isInitialized = true;
  }

  public boolean equals(Object object) {
    if (this == object) return true;
    if (object == null || getClass() != object.getClass()) return false;
    if (!super.equals(object)) return false;
    VideoPlayer that = (VideoPlayer) object;
    return nodeId == that.nodeId && fabricIndex == that.fabricIndex;
  }

  public int hashCode() {
    return Objects.hash(super.hashCode(), nodeId, fabricIndex);
  }

  @Override
  public String toString() {
    return "VideoPlayer{"
        + "nodeId="
        + nodeId
        + ", fabricIndex="
        + fabricIndex
        + ", deviceName='"
        + deviceName
        + '\''
        + ", vendorId="
        + vendorId
        + ", productId="
        + productId
        + ", deviceType="
        + deviceType
        + ", contentApps="
        + contentApps
        + ", isConnected="
        + isConnected
        + ", numIPs="
        + numIPs
        + ", ipAddresses="
        + ipAddresses
        + ", hostName='"
        + hostName
        + '\''
        + ", instanceName='"
        + instanceName
        + '\''
        + ", lastDiscoveredMs="
        + lastDiscoveredMs
        + ", MACAddress='"
        + MACAddress
        + '\''
        + ", isAsleep="
        + isAsleep
        + ", port="
        + port
        + ", isInitialized="
        + isInitialized
        + '}';
  }

  CastingPlayer getCastingPlayer() {
    return castingPlayer;
  }

  public long getNodeId() {
    return nodeId;
  }

  public byte getFabricIndex() {
    return fabricIndex;
  }

  public boolean isConnected() {
    return isConnected;
  }

  public List<ContentApp> getContentApps() {
    return contentApps;
  }

  public String getDeviceName() {
    return deviceName;
  }

  public int getVendorId() {
    return vendorId;
  }

  public int getProductId() {
    return productId;
  }

  public int getDeviceType() {
    return deviceType;
  }

  public int getNumIPs() {
    return numIPs;
  }

  public List<InetAddress> getIpAddresses() {
    return ipAddresses;
  }

  public String getHostName() {
    return hostName;
  }

  public int getPort() {
    return port;
  }

  public long getLastDiscoveredMs() {
    return lastDiscoveredMs;
  }

  public void setLastDiscoveredMs(long lastDiscoveredMs) {
    this.lastDiscoveredMs = lastDiscoveredMs;
  }

  public String getMACAddress() {
    return MACAddress;
  }

  public String getInstanceName() {
    return instanceName;
  }

  public void setIsAsleep(boolean asleep) {
    isAsleep = asleep;
  }

  public boolean isAsleep() {
    return isAsleep;
  }

  public boolean isInitialized() {
    return isInitialized;
  }
}
