//========================================
//================UDP FXNS================
//========================================
//---PARSE UDP INPUT
void parseUDP(String thisudp){
  int comma1Index = thisudp.indexOf(','); //---DETERMINE IF SYSTEM COMMAND (7)
  udpcmd[0] = (thisudp.substring(0, comma1Index)).toInt();
  if (udpcmd[0] == 7){
    dbo("   recieved UDP SYSTEM COMMAND (7) - Writing to EEPROM ");
    int comma2Index = thisudp.indexOf(',', comma1Index+1);
    int comma3Index = thisudp.indexOf(',', comma2Index+1);
    String cfgname = thisudp.substring(comma1Index+1, comma2Index);
    String cfgval  = thisudp.substring(comma2Index+1, comma3Index);
    dbo(cfgname); dbo("="); dbo(cfgval); dbo("\n");
    //eeWriteString(cfgname, cfgval);
    blinkInteger(8,250);
  }
  else{
    int comma2Index = thisudp.indexOf(',', comma1Index+1);
    int comma3Index = thisudp.indexOf(',', comma2Index+1);
    int comma4Index = thisudp.indexOf(',', comma3Index+1);  
    udpcmd[1] = (thisudp.substring(comma1Index+1, comma2Index)).toInt();
    udpcmd[2] = (thisudp.substring(comma2Index+1, comma3Index)).toInt();
    udpcmd[3] = (thisudp.substring(comma3Index+1, comma4Index)).toInt();
    udpcmd[4] = (thisudp.substring(comma4Index+1)).toInt();
    dbo("   parsed command: "); dbo(String(udpcmd[0])); dbo("|"); dbo(String(udpcmd[1])); dbo("|"); dbo(String(udpcmd[2])); dbo("|"); dbo(String(udpcmd[3])); dbo("|"); dbo(String(udpcmd[4])); dbo("\n"); 
  }
}

//---EXECUTE UDP COMMAND
//---   mode8 = LED (M, i, r, g, b) //---"8,2,128,64,24"
//---   mode5 = ServoPWM (M, s0, s1, X, X); //---"5,4800,4800,0,0"
void execUDP(){
  if (udpcmd[0] == 5){ //---SERVO RAW MODE (servo commands in PWM (~3000-6000), no checks)
    dbo("---SERVO RAW MODE (M,s0,s1,X,X): ");
    dbo("s0:"); dbo(String(udpcmd[1])); dbo(" s1:"); dbo(String(udpcmd[2])); dbo("\n");
        setServos(udpcmd[1],udpcmd[2]);
  }    
  if (udpcmd[0] == 8){ //---LED COMMAND MODE
    dbo("---LED COMMAND MODE (M,i,r,g,b) (modified for ESP32-CAM just flash (M, brightness) - "); dbo(String(udpcmd[1])); dbo("\n");
    setFlash(udpcmd[1]);
  }
}
