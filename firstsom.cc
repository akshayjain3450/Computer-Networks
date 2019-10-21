/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int
main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);

  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NodeContainer nodes;
  nodes.Create (4);

  InternetStackHelper stack;
  stack.Install (nodes);

  PointToPointHelper pointToPoint1;
  pointToPoint1.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint1.SetChannelAttribute ("Delay", StringValue ("2ms"));

  PointToPointHelper pointToPoint2;
  pointToPoint2.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint2.SetChannelAttribute ("Delay", StringValue ("2ms"));

  PointToPointHelper pointToPoint3;
  pointToPoint3.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint3.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer devices1;
  devices1 = pointToPoint1.Install (nodes.Get(0),nodes.Get(1));

  NetDeviceContainer devices2;
  devices2 = pointToPoint2.Install (nodes.Get(1),nodes.Get(2));

  NetDeviceContainer devices3;
  devices3 = pointToPoint3.Install (nodes.Get(1),nodes.Get(3));


  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4AddressHelper address1;
  address1.SetBase ("10.1.2.0", "255.255.255.0");

  Ipv4AddressHelper address2;
  address2.SetBase ("10.1.3.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces1 = address.Assign (devices1);
  Ipv4InterfaceContainer interfaces2 = address1.Assign (devices2);
  Ipv4InterfaceContainer interfaces3 = address2.Assign (devices3);

  UdpEchoServerHelper echoServer (999);
  ApplicationContainer serverApps = echoServer.Install (nodes.Get (3));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));
  UdpEchoClientHelper echoClient1 (interfaces3.GetAddress (1), 999);
  echoClient1.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient1.SetAttribute ("Interval", TimeValue (Seconds (0.01)));
  echoClient1.SetAttribute ("PacketSize", UintegerValue (1024));
  ApplicationContainer clientApps1 = echoClient1.Install (nodes.Get (2));
  clientApps1.Start (Seconds (1.0));
  clientApps1.Stop (Seconds (10.0));

Ipv4GlobalRoutingHelper::PopulateRoutingTables();

  //  UdpEchoServerHelper echoServer1 (10);
  //UdpEchoServerHelper echoServer2 (11);

  UdpEchoServerHelper echoServer1 (1000);
  ApplicationContainer serverApps1 = echoServer1.Install (nodes.Get (2));
  serverApps1.Start (Seconds (1.0));
  serverApps1.Stop (Seconds (10.0));
  UdpEchoClientHelper echoClient2 (interfaces2.GetAddress (1), 1000);
  echoClient2.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient2.SetAttribute ("Interval", TimeValue (Seconds (0.01)));
  echoClient2.SetAttribute ("PacketSize", UintegerValue (1024));
  ApplicationContainer clientApps2 = echoClient2.Install (nodes.Get (0));
  clientApps2.Start (Seconds (1.0));
  clientApps2.Stop (Seconds (10.0));






  //ApplicationContainer clientApps3 = echoClient3.Install (nodes.Get ());




  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
