//
// Copyright (C) 2011 David Eckhoff <eckhoff@cs.fau.de>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "TestWaveApplLayer.h"
#include <algorithm>

Define_Module(TestWaveApplLayer);

void TestWaveApplLayer::initialize(int stage) {
	BaseWaveApplLayer::initialize(stage);
	receivedBeacons = 0;
	receivedData = 0;
}

void TestWaveApplLayer::onBeacon(WaveShortMessage* wsm) {

    std::string sendername=wsm->getPsc();
    cPacket *m = wsm->decapsulate();
    if (simTime()>=1){
    if (attacker_type==0){ //This part of code is for the victime code. It saves information about each received beacon from all vehicles
    attacker.open(concatenate("C:\\Users\\abdelwahab.boualouac\\Desktop\\Simulation\\Results\\ML_Passive_Attacker\\Test\\",myname.c_str(),".txt"),std::ios::app);
    double d=abs(wsm->getSenderPos().x-curXposition);
    attacker << simTime() << ";" << myId <<";" << wsm->getSenderAddress()<<";" << curXposition<<";"<< d << std::endl;
    attacker.close();
    }
    else{// This part is for the attacker to adapt its speed according to the victim's speed
        if (wsm->getSenderAddress()==0){ // This is for capture beacons only from the victim
        double d=abs(wsm->getSenderPos().x-curXposition); // This is calculate the distance from the victim 
		
		///This bloc is saves information about the attacker on a file, 
        attacker.open(concatenate("C:\\Users\\abdelwahab.boualouac\\Desktop\\Simulation\\Results\\ML_Passive_Attacker\\Test\\",myname.c_str(),".txt"),std::ios::app);
        attacker << simTime() << ";" << myId <<";" << wsm->getSenderAddress()<<";" << curXposition<<";"<< d << std::endl;
        attacker.close();
       
	   // The type of this attacker is a contant attacker
        if(type==1){
            if(d>110){ // the distance from the victim is greater then then CD + RD = 100m + 10m , it increases its speeds
        currentspeed = traci->getSpeed();
        traci->commandSetSpeed(currentspeed+1);
            }
        else{
            if(d<100){ // the distance from the victim is less then CD= 100m, it decreases its speeds
                    currentspeed = traci->getSpeed();
                    traci->commandSetSpeed(currentspeed-1);
                    }
            else {

                traci->commandSetSpeed(20); //if the distance is between CD (100m) and CD+RD (110m), it keeps a constant speed (the speed of the victim)
            }
        }
        }
        }
    }
    }
}

void TestWaveApplLayer::onData(WaveShortMessage* wsm) {


	}



void TestWaveApplLayer::handlePositionUpdate(cObject* obj) {
    BaseWaveApplLayer::handlePositionUpdate(obj);


}

char *TestWaveApplLayer::concatenate(const char *a, const char *b, const char *c) {
    size_t alen = strlen(a);
    size_t blen = strlen(b);
    size_t clen = strlen(c);
    char *res = (char *)malloc(alen + blen + clen + 1);
    if (res) {
        memcpy(res, a, alen);
        memcpy(res + alen, b, blen);
        memcpy(res + alen + blen, c, clen + 1);
    }
    return res;
}

TestWaveApplLayer::~TestWaveApplLayer() {

}


