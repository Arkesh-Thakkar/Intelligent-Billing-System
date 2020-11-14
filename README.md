# Intelligent-Billing-System
IoT based solution prototyping the smart trolley based system for reducing the billing time in the shopping mall by auto-billing of products within the trolley without manual scanning with the help of RFID. 


Pre-requisites:
--> Node MCU
--> RFID tag, RFID reader, breadboard, jumperwires.
--> Ardunio IDE
--> AWS RDS service

Simulation:

Run "Enter data to RFID" (It simulates the warehousing part where product details and its proice are entered to sell):
--> Provide netword credentials and password.
--> Enter the url provided by it in browser.
--> It opens up an html form having fields of product name and product price.
--> Enter the details within form and submit it.
-->Scan rfid tag to rfid reader.
--> Now the data is inserted within tag.

Then Run "Insert in db" file (It simulates the environment for the customer where whenever customer enters the product having rfid into the trolley the trolley would contain rfid reader and nodemcu with this code which reads the name, price and id of the tag and enters into the database by autoscanning it and also deleting the extra items if once scanned and then removed from trolley. Thus it can save considerable amount of time for billing).
--> Provide network credentials and assword.
--> Also provide AWS RDS credentials along with the correct database and table name.
--> Scan the rfid tag havig the information stored above and it would enter thwe information within the database.
