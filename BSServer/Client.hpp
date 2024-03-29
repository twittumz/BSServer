#pragma once
#include <iostream>
#include <openssl/ssl.h>
#include <sys/socket.h>
#include <list>
#include <vector>
#include <string>
#include "Helper.hpp"
#include "Weapon.hpp"
struct CommandMessage{
       CommandMessage(std::string c,unsigned long t){
           cmd = c;
           timestamp=t;
       }
    std::string cmd;
    unsigned long timestamp;
};
class Client{
  private: 
  //SSL * _ssl;

  int socket;
  int packets_sent=0;
  int packet_sequence=0;
  float x=0,y=0;
  double angle=0;
  bool moving =false;
  bool joined=false;
  unsigned long lastResponce = 0;
  unsigned long lastPing = 0;
  unsigned long pingTimeSent=0;
  unsigned long pongTimeReceived=0;
  unsigned long pingsSent=0;
  unsigned long totalPing=0;
  unsigned long latency=0;
  
  
  bool dead=false;
  float deathCounter=0.f;
  float respawnTime=19.f;
  unsigned long lastDeathUpdate = 0;
  
  float kills=0;
  float deaths=0;
  float health=100;
  float max_health=100;
  Weapon *weapon;
  
  unsigned long lastUpdate=0;
  std::list<CommandMessage> * commands;
  std::string playerName="player";
  public:  
  Client(int nsock){
     // _ssl=s;
      socket=nsock;
  commands = new std::list<CommandMessage>();
  }  
    Client(){
          commands = new std::list<CommandMessage>();
    }
 /* SSL* ssl(){
      return _ssl;
  }*/
    int getConnection(){
        return socket;
    }
    void move(float _x,float _y){
        x=_x;
        y=_y;
    }
    void setX(float _x){
        x=_x;
    }
    void setY(float _x){
        y=_x;
    }
    void setMoving(bool mv){
        moving=mv;
    }
    bool isMoving(){
        return moving;
    }
    void setAngle(double _angle){
        angle=_angle;
    }
    float getX(){
        return x;
    }
     float getY(){
        return y;
    }
    double getAngle(){
        return angle;
    }
    void incPackets_sent(){
        packets_sent++;
    }
    int getPacketsSent(){
        return packets_sent;
    }
    void setSequenceId(int id){
       packet_sequence=id;
    }
    int getSequenceId(){
        return packet_sequence;
    }
    unsigned long getLastResponce(){
        return lastResponce;
    }
    void setLastResponce(unsigned long rp){
        lastResponce=rp;
       
    }
    
    unsigned long getLastPing(){
        return lastPing;
    }
    void setLastPing(unsigned long rp){
        lastPing=rp;
    }
    
    unsigned long getPingTimeSent(){
        return pingTimeSent;
    }
    void setPingTimeSent(unsigned long rp){
        pingTimeSent=rp;
    }

       unsigned long getPongTimeReceived(){
        return pongTimeReceived;
    }
    
        void setLastUpdate(unsigned long rp){
        lastUpdate=rp;
    }

       unsigned long getLastUpdate(){
        return lastUpdate;
    }
    void setPongTimeReceived(unsigned long rp){
        pongTimeReceived=rp;
       totalPing+= getPongTimeReceived()-getLastPing();
       pingsSent++;
       latency = pongTimeReceived - getLastPing();
    }
    int getLatency(){
        return latency;
    }
     int getAverageLatency(){
        return (pingsSent>0)? totalPing/pingsSent:0;
    }
    void pushCommand(std::string cmd){
      //  commands->push_back(cmd);
        CommandMessage  cm(cmd,Helper::getTime()-(getPongTimeReceived()-getLastPing()));
        commands->push_back(cm);
     
        
    }
    int commands_size(){
        return commands->size();
    }
    
   CommandMessage popCommand(){
          CommandMessage cmd = commands->front();
        commands->pop_front();
        return cmd;
    }
    
    std::string getName(){
        return playerName;
    }
    void setName(std::string name){
        playerName=name;
    }
    void kill(){
        dead=true;
          deathCounter=respawnTime;
          lastDeathUpdate= Helper::getTime();
    }
    void setDead(bool _dead){
        dead=_dead;
        if(dead){
            deathCounter=respawnTime;
            lastDeathUpdate= Helper::getTime();
        }
    }
    unsigned long getLastDeathUpdate(){
        return   lastDeathUpdate;
    }
    void setLastDeathUpdate(unsigned long t){
        lastDeathUpdate=t;
    }
    //returns true if its time for the player to respawn
    bool updateDeathCunter(){
        unsigned long ct = Helper::getTime();
        long dt =  (ct-lastDeathUpdate);
          deathCounter-=  (float)(dt/1000.f);
          lastDeathUpdate=ct;
         
         
          if(deathCounter<=0){
              //respawn;
               dead=false;
              deathCounter=0;
              return true;
           }
           return false;
    }
    bool isDead(){
        return dead;
    }
    void respawn(float _x, float _y){
        move(_x,_y);
        dead=false;
        deathCounter=0.f;
    }
    void incDeaths(){
        deaths++;
    }
    void incKills(){
        kills++;
    }
    bool didJoin(){
        return joined;
    }
    void setJoined(bool j){
        joined=j;
    }
    float getHealth(){
        return health;
    }
    void reset(){
        dead=false;
        health=max_health;
        joined=false;
        deaths=0;
        kills=0;
        moving=false;
        commands->clear();
    }
    socklen_t clilen;
  struct sockaddr_in  cli_addr;
    
};