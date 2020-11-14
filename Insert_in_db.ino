/*
 * Initial Author: ryand1011 (https://github.com/ryand1011)
 *
 * Reads data written by a program such as "rfid_write_personal_data.ino"
 *
 * See: https://github.com/miguelbalboa/rfid/tree/master/examples/rfid_write_personal_data
 *
 * Uses MIFARE RFID card using RFID-RC522 reader
 * Uses MFRC522 - Library
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
*/

#include <SPI.h>
#include <MFRC522.h>
//db1
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

//db1 end

#define RST_PIN         D2           // Configurable, see typical pin layout above
#define SS_PIN          D4          // Configurable, see typical pin layout above


//db2
//char pname[50],value[50];
char ssid[] = "network_name";                 // Network Name
char pass[] = "password";                 // Network Password

WiFiServer server(80);
//db2end

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance


//db3

char query[128];
//Connector my_conn;
IPAddress server_addr(aws_cloud_ip: x,x,x,x);          // MySQL server IP
char user[] = "admin";           // MySQL user
char password[] = "admin1234";       // MySQL password
//char INSERT_SQL[] = "INSERT INTO de_arduino.de_table1(pname,value) VALUES ('"pname"','"value+"')";
//char INSERT_SQL[] = "INSERT INTO de_arduino.de_table1(message) VALUES ('Arduino')";

WiFiClient client;
MySQL_Connection conn((Client *)&client);


//db3end


//*****************************************************************************************//
void setup() {
  Serial.begin(115200);                                           // Initialize serial communications with the PC
  SPI.begin();                                                  // Init SPI bus
  mfrc522.PCD_Init();                                              // Init MFRC522 card
  Serial.println(F("Read personal data on a MIFARE PICC:"));    //shows in serial that it is ready to read


//db4

Serial.println("Initialising connection");
  Serial.print(F("Setting static ip to : "));
//  Serial.println(ip);

  Serial.println("");
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid); 
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print("$");
  }

  Serial.println("");
  Serial.println("WiFi Connected");

 Serial.print("Assigned IP: ");
  Serial.print(WiFi.localIP());
  Serial.println("");

  Serial.println("Connecting to database");

  while (!conn.connect(server_addr, 3306, user, password)) {
    delay(200);
    Serial.print ( "." );
  }

  Serial.println("");
  Serial.println("Connected to SQL Server!");  


//db4end



}


//*****************************************************************************************//
void loop() {


  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  //some variables we need
  byte block;
  byte len;
  MFRC522::StatusCode status;

  //-------------------------------------------

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.println(F("**Card Detected:**"));

  //-------------------------------------------

  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card
  //byte uuiidd[10]=mfrc522.uid.uidByte();
  
  //Serial.println(mfrc522.uid.uidByte);
  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));      //uncomment this to see all blocks in hex

  //-------------------------------------------



//Uid

char uuiid[20]="\0";



Serial.println("hi your uid is  ");

int i;
/*for (i = 0; i < 4; i++) {  
    uuiid[i] = mfrc522.uid.uidByte[i];
    Serial.print(uuiid[i]);
    
    Serial.println();
   
    Serial.print(uuiid[i],HEX);

}*/



String rfid_uid = "";

    for (int i = 0; i < 4; i++) {
        String uid_part = String(mfrc522.uid.uidByte[i], HEX); 
        Serial.print(uid_part);
        rfid_uid += uid_part;
    }

    Serial.println();

//rest as before

rfid_uid = "'"+rfid_uid+"'";
rfid_uid.toCharArray(uuiid,20);


  
  Serial.print(F("Name: "));

  byte buffer1[36];


//  null buffer1

  for(int i=0;i<sizeof(buffer1);i++)
  {
     buffer1[i]='\0';
  }


//  end null buffer1







  block = 4;
  len = 36;

  //------------------------------------------- GET FIRST NAME
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer1, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
    
  //PRINT FIRST NAME
  for (uint8_t i = 0; i < 16; i++)
  {
    if (buffer1[i] != 32)
    {
      Serial.write(buffer1[i]);
    }
  }
  Serial.print(" ");

  //---------------------------------------- GET LAST NAME



//Reading from sector1 block5
  byte buffer11[18];

  block = 5;
  len = 18;

  //------------------------------------------- GET FIRST NAME
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer11, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
    
  //PRINT FIRST NAME
  for (uint8_t i = 0; i < 16; i++)
  {
    if (buffer11[i] != 32)
    {
      Serial.write(buffer11[i]);
    }
  }
  Serial.print(" ");

  //---------------------------------------- GET LAST NAME

//end reading from sector1 block5







  byte buffer2[18];
  block = 8;

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 8, &key, &(mfrc522.uid)); //line 834
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer2, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  //PRINT LAST NAME
  for (uint8_t i = 0; i < 16; i++) {
    Serial.write(buffer2[i] );
  }







//appending buffer1 and buffer11

for(int i=16;i<36;i++)
{
  buffer1[i]=buffer11[i-16];
}






//end appending
  
  char pname[50]="\0",value[50]="\0";

  for(int i=0;i<=sizeof(buffer1);i++)
  {
    pname[i] = (char )buffer1[i];
    //value[i] = (char )buffer2[i];  
  }
  for(int i=0;i<=sizeof(buffer2);i++)
  {
    value[i]=(char)buffer2[i];
  }


 
  String pname1 = "'"+(String)pname+"'";
  String value1 = "'"+(String)value+"'";

  
  pname1.toCharArray(pname,50);
  value1.toCharArray(value,50);
  Serial.println(pname);



  Serial.println(F("\n**End Reading**\n"));
 // Serial.println(F(pname1));
  //db5
//char INSERT_SQL[50];
char INSERT_SQL[] = "INSERT INTO de_table1.de_table_table1(uid,pname,value) VALUES (%s,%s,%s)";

sprintf(query, INSERT_SQL,uuiid,pname,value);
 
 Serial.println("Recording data.");
 Serial.println(query);
  
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  
  cur_mem->execute(query);


/*
// searching from db starts

char query1[] = "SELECT uid FROM de_table1.de_table_table1";
 row_values *row = NULL;
 String uid_db[20] = "\0";
 char uid_db_item[20]="\0";
  int item=0;
 
  cur_mem->execute(query1);
  // Fetch the columns (required) but we don't use them.
  column_names *columns = cur_mem->get_columns();
 
  // Read the row (we are only expecting the one)
  do {
    row = cur_mem->get_next_row();
    if (row != NULL) {
      Serial.println("Checking dataaaa...");
      uid_db[item++]=(String)row->values[0];
     
      Serial.println("All ok yeeaah");
    }
  } while (row != NULL);


   boolean exist=false;
   char delete_query[]="Delete from de_table1.de_table_table1 where uid like %s";
   for(int i=0;i<item;i++)
   {
    Serial.println(uid_db[0]);
       if(uid_db[item].toCharArray()==uuiid)
      {
        Serial.println("changing flag status");
        exist = true;
        break;      
      }
   }

  if(exist==false)
  {
    Serial.println("Deleting");
    sprintf(delete_query, delete_query,uid_db[0]);
    cur_mem->execute(delete_query);
  }


*/




// searching  db ends



  delete cur_mem;


  
  //db5 end

  

  delay(100); //change value if you want to read cards faster

  //mfrc522.PICC_HaltA();   // After commenting this infinite scanning starts.
  mfrc522.PCD_StopCrypto1();
}
//*****************************************************************************************//
