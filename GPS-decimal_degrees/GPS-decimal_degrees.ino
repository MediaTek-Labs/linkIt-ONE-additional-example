#include <LGPS.h>

gpsSentenceInfoStruct g_info;

void getGPSData(gpsSentenceInfoStruct &g_info, char* GPS_formatted)
{
  LGPS.powerOn();
  boolean GPS_fix = false;

  while (!GPS_fix)
  {
    LGPS.getData(&g_info);                                      //get the data from the GPS and store it in 'g_info'
    GPS_fix = printGPGGA((char*)g_info.GPGGA,GPS_formatted);    //printGPGGA returns TRUE if the GPGGA string returned confirms a GPS fix.
  }
  LGPS.powerOff();
}

boolean printGPGGA(char* str, char* GPS_formatted)
{
  char SMScontent[160];
  char latitude[20];
  char lat_direction[1];
  char longitude[20];
  char lon_direction[1];
  char buf[20];
  char time[30];
  const char* p = str;
  p = nextToken(p, 0); // GGA
  p = nextToken(p, time); // Time
  p = nextToken(p, latitude); // Latitude
  p = nextToken(p, lat_direction); // N or S?
  p = nextToken(p, longitude); // Longitude
  p = nextToken(p, lon_direction); // E or W?
  p = nextToken(p, buf); // fix quality
  if (buf[0] == '1')
  {
    // GPS fix
    p = nextToken(p, buf); // number of satellites
    Serial.print("GPS is fixed:");
    Serial.print(atoi(buf));
    Serial.println(" satellite(s) found!");
    strcpy(SMScontent, "GPS fixed, satellites found: ");
    strcat(SMScontent, buf);
    
    const int coord_size = 8;
    char lat_fixed[coord_size],lon_fixed[coord_size];
    convertCoords(latitude,longitude,lat_fixed, lon_fixed,coord_size);
    
    Serial.print("Latitude:");
    Serial.println(lat_fixed);
    Serial.println(lat_direction);
    
    Serial.print("Longitude:");
    Serial.println(lon_fixed);
    Serial.println(lon_direction);
    
    char gmaps_buffer[50];
    sprintf(gmaps_buffer,"\nhttp://maps.google.com/?q=%s%s,%s%s",lat_fixed,lat_direction,lon_fixed,lon_direction);
    
    strcat(SMScontent,gmaps_buffer);
    
    Serial.print("Time: ");
    Serial.println(time);
    strcat(SMScontent, "\nTime: ");
    strcat(SMScontent,time);
    
    strcpy(GPS_formatted, SMScontent);
    return true;
  }
  else
  {
    Serial.println("GPS is not fixed yet.");
    return false;
  }
}

void convertCoords(const char* latitude, const char* longitude, char* lat_return, char* lon_return, int buff_length)
{
  char lat_deg[3];
  strncpy(lat_deg,latitude,2);      //extract the first 2 chars to get the latitudinal degrees
  lat_deg[2] = 0;                   //null terminate
  
  char lon_deg[4];
  strncpy(lon_deg,longitude,3);      //extract first 3 chars to get the longitudinal degrees
  lon_deg[3] = 0;                    //null terminate
  
  int lat_deg_int = arrayToInt(lat_deg);    //convert to integer from char array
  int lon_deg_int = arrayToInt(lon_deg);
  
  // must now take remainder/60
  //this is to convert from degrees-mins-secs to decimal degrees
  // so the coordinates are "google mappable"
  
  float latitude_float = arrayToFloat(latitude);      //convert the entire degrees-mins-secs coordinates into a float - this is for easier manipulation later
  float longitude_float = arrayToFloat(longitude);
  
  latitude_float = latitude_float - (lat_deg_int*100);      //remove the degrees part of the coordinates - so we are left with only minutes-seconds part of the coordinates
  longitude_float = longitude_float - (lon_deg_int*100);
  
  latitude_float /=60;                                    //convert minutes-seconds to decimal
  longitude_float/=60;
  
  latitude_float += lat_deg_int;                          //add back on the degrees part, so it is decimal degrees
  longitude_float+= lon_deg_int;
  
  snprintf(lat_return,buff_length,"%2.3f",latitude_float);    //format the coordinates nicey - no more than 3 decimal places
  snprintf(lon_return,buff_length,"%3.3f",longitude_float);
}

int arrayToInt(const char* char_array)
{
  int temp;
  sscanf(char_array,"%d",&temp);
  return temp;
}

float arrayToFloat(const char* char_array)
{
  float temp;
  sscanf(char_array, "%f", &temp);
  return temp;
}

const char *nextToken(const char* src, char* buf)
{
  int i = 0;
  while (src[i] != 0 && src[i] != ',')
  i++;
  if (buf)
  {
    strncpy(buf, src, i);
    buf[i] = 0;
  }
  if (src[i])
  i++;
  return src + i;
}

void setup()
{
  Serial.begin(9600); 
}

void loop()
{
  delay(5000); 
  char GPS_formatted[130]; 
  getGPSData(g_info,GPS_formatted);
}

