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
#include "ns3/netanim-module.h"
#include "ns3/flow-monitor-module.h"
#include <iostream>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");


void
printStats (FlowMonitorHelper &flowmon_helper, bool perFlowInfo) {
  Ptr<Ipv4FlowClassifier> classifier =
DynamicCast<Ipv4FlowClassifier>(flowmon_helper.GetClassifier());
  std::string proto;
  Ptr<FlowMonitor> monitor = flowmon_helper.GetMonitor ();
  std::map < FlowId, FlowMonitor::FlowStats > stats = monitor->GetFlowStats();
  double totalTimeReceiving;
  uint64_t totalPacketsReceived, totalPacketsDropped,
totalBytesReceived,totalPacketsTransmitted;

  totalBytesReceived = 0, totalPacketsDropped = 0,
totalPacketsReceived = 0, totalTimeReceiving =
0,totalPacketsTransmitted = 0;
  for (std::map< FlowId, FlowMonitor::FlowStats>::iterator flow =
stats.begin(); flow != stats.end(); flow++)
  {
    Ipv4FlowClassifier::FiveTuple  t = classifier->FindFlow(flow->first);
    switch(t.protocol)
     {
     case(6):
         proto = "TCP";
         break;
     case(17):
         proto = "UDP";
         break;
     default:
         exit(1);
     }
     totalBytesReceived += (double) flow->second.rxBytes * 8;
     totalTimeReceiving += flow->second.timeLastRxPacket.GetSeconds ();
     totalPacketsReceived += flow->second.rxPackets;
     totalPacketsDropped += flow->second.txPackets - flow->second.rxPackets;
     totalPacketsTransmitted += flow->second.txPackets;
     if (perFlowInfo)
     {
      std::cout << "FlowID: " << flow->first << " (" << proto << " "
                 << t.sourceAddress << " / " << t.sourcePort << " --> "
                 << t.destinationAddress << " / " << t.destinationPort << ")" << std::endl;
       std::cout << "  Tx Bytes: " << flow->second.txBytes << std::endl;
std::cout << "  Lost Packets: " << flow->second.lostPackets << std::endl;
       std::cout << "  Pkt Lost Ratio: " <<((double)flow->second.txPackets-(double)flow->second.rxPackets)/(double)flow->second.txPackets << std::endl;
       std::cout << "  Throughput: " << (((double)flow->second.rxBytes * 8) /(flow->second.timeLastRxPacket.GetSeconds ()) ) << "bits/s" <<std::endl;
       std::cout << "  Mean{Delay}: " <<(flow->second.delaySum.GetSeconds()/flow->second.rxPackets) <<std::endl;
       std::cout << "  Mean{Jitter}: " <<(flow->second.jitterSum.GetSeconds()/(flow->second.rxPackets)) <<std::endl;
     }
}

     std::cout<< "Final throughput with (packets received/total time): "<<(totalPacketsReceived * 1024 * 8)/totalTimeReceiving<<" bps "<<std::endl;
     std::cout<< "Final throughput with (packets received/total time): "<<(totalBytesReceived)/totalTimeReceiving<<" bps "<<std::endl;
     std::cout<<"Total packets transmitted:"<<totalPacketsTransmitted<<std::endl;
     std::cout<<"Total packets received: "<< totalPacketsReceived<<std::endl;
     std::cout<<"Total packets dropped: "<< totalPacketsDropped<<std::endl;
     std::cout << "Packet Lost Ratio: " << totalPacketsDropped /(double) (totalPacketsReceived + totalPacketsDropped) << std::endl;

}
int
main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);

  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NodeContainer nodes;
  nodes.Create (4); //nodes.Get(0), nodes.Get(1), nodes.Get(2), nodes.Get(3)

  InternetStackHelper stack;
  stack.Install (nodes);

  PointToPointHelper pointToPoint1;
  pointToPoint1.SetDeviceAttribute ("DataRate", StringValue ("100Mbps"));
  pointToPoint1.SetChannelAttribute ("Delay", StringValue ("10ms"));

  PointToPointHelper pointToPoint2;
  pointToPoint2.SetDeviceAttribute ("DataRate", StringValue ("1024Mbps"));
  pointToPoint2.SetChannelAttribute ("Delay", StringValue ("10ms"));

  PointToPointHelper pointToPoint3;
  pointToPoint3.SetDeviceAttribute ("DataRate", StringValue ("50Mbps"));
  pointToPoint3.SetChannelAttribute ("Delay", StringValue ("10ms"));

  Ipv4AddressHelper address;

  Ipv4InterfaceContainer interfaces;

  NetDeviceContainer devices;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  devices = pointToPoint1.Install (nodes.Get(0), nodes.Get(1));
  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  address.SetBase ("10.1.3.0", "255.255.255.0");
  devices = pointToPoint3.Install (nodes.Get(1), nodes.Get(3));
  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  UdpEchoServerHelper echoServer1 (9);

  ApplicationContainer serverApps1 = echoServer1.Install (nodes.Get (3));
  serverApps1.Start (Seconds (1.0));
  serverApps1.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient1 (interfaces.GetAddress (1), 9);
  echoClient1.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient1.SetAttribute ("Interval", TimeValue (Seconds (0.01)));
  echoClient1.SetAttribute ("PacketSize", UintegerValue (1024));

  address.SetBase ("10.1.2.0", "255.255.255.0");
  devices = pointToPoint2.Install (nodes.Get(1), nodes.Get(2));
  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  ApplicationContainer clientApps1 = echoClient1.Install (nodes.Get (2));
  clientApps1.Start (Seconds (2.0));
  clientApps1.Stop (Seconds (10.0));

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  UdpEchoServerHelper echoServer2 (10);

  ApplicationContainer serverApps2 = echoServer2.Install (nodes.Get (2));
  serverApps2.Start (Seconds (1.0));
  serverApps2.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient2 (interfaces.GetAddress (1), 9);
  echoClient2.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient2.SetAttribute ("Interval", TimeValue (Seconds (0.01)));
  echoClient2.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps2 = echoClient2.Install (nodes.Get (0));
  clientApps2.Start (Seconds (5.0));
  clientApps2.Stop (Seconds (15.0));

  AnimationInterface anim ("animation_test.xml");
  anim.SetConstantPosition (nodes.Get(0), 10.0, 30.0);
  anim.SetConstantPosition (nodes.Get(1), 50.0, 30.0);
  anim.SetConstantPosition (nodes.Get(2), 90.0, 30.0);
  anim.SetConstantPosition (nodes.Get(3), 50.0, 70.0);

  Ptr<FlowMonitor> flowmon;
  FlowMonitorHelper flowmonHelper;
  flowmon = flowmonHelper.InstallAll ();
  Simulator::Stop (Seconds(10.1));
  Simulator::Run ();

  printStats (flowmonHelper, true);

  Simulator::Destroy ();
  return 0;
}
