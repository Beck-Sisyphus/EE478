void init_sched(){
    for(int i = 0; i < 256; i++){
        schedule[i] = 0;
    }
}

void generateHTML(WiFiClient client) {
    // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
    // and a content-type so the client knows what's coming, then a blank line:
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();

    // the content of the HTTP response follows the header:
    client.println("<html><head><title>Green GOLEM</title></head><body align=center>");
    client.println("<h1 align=center><font color=\"red\">Green GOLEM</font></h1>");
    client.print("RED LED <button onclick=\"location.href='/LED/H'\">HIGH</button>");
    client.println(" <button onclick=\"location.href='/LED/L'\">LOW</button><br>");

    // Need to control 5 remotes, and 3 alarms
    client.print("Remote I <button onclick=\"location.href='/RemoteI/H'\">HIGH</button>");
    client.println(" <button onclick=\"location.href='/RemoteI/L'\">LOW</button><br>");
    client.print("Remote II <button onclick=\"location.href='/RemoteII/H'\">HIGH</button>");
    client.println(" <button onclick=\"location.href='/RemoteII/L'\">LOW</button><br>");
    client.print("Remote III <button onclick=\"location.href='/RemoteIII/H'\">HIGH</button>");
    client.println(" <button onclick=\"location.href='/RemoteIII/L'\">LOW</button><br>");
    client.print("Remote IV <button onclick=\"location.href='/RemoteIV/H'\">HIGH</button>");
    client.println(" <button onclick=\"location.href='/RemoteIV/L'\">LOW</button><br>");
    client.print("Remote V <button onclick=\"location.href='/RemoteV/H'\">HIGH</button>");
    client.println(" <button onclick=\"location.href='/RemoteV/L'\">LOW</button><br>");

    client.print("Smoke Alarm <button onclick=\"location.href='/AlarmSmoke/H'\">HIGH</button>");
    client.println(" <button onclick=\"location.href='/AlarmSmoke/L'\">LOW</button><br>");
    client.print("Door Alarm <button onclick=\"location.href='/AlarmBroker/H'\">HIGH</button>");
    client.println(" <button onclick=\"location.href='/AlarmBroker/L'\">LOW</button><br>");
    client.print("Temp Alarm <button onclick=\"location.href='/AlarmTemp/H'\">HIGH</button>");
    client.println(" <button onclick=\"location.href='/AlarmTemp/L'\">LOW</button><br>");
    
    // Sensors readouts
    client.print("Humidity ():  ");
    client.print(humidity);
    client.println("<br>");
    client.print("Temperature (oC):  ");
    client.print(celsius);
    client.println("<br>");
    client.print("Temperature (oF): ");
    client.print(fahrenheit);
    client.println("<br>");
    client.print("Temperature (K): ");
    client.print(kelvin);
    client.println("<br>");
    client.print("Dew Point (oC): ");
    client.print(dewpoint);
    client.println("<br>");
    client.print("Dew Point Slow (oC): ");
    client.print(dewpointslow);
    client.println("<br>");

    // The website Mason made
    client.println("<form>");
    client.println("<div align =\"center\">");
    client.println("Schedule an Event: <br> <br>");
    client.println("Date:<br>");
        
    client.println("<select name=\"M\">");
    client.println("<option value=-1>M</option>");
    client.println("<option value=01>1</option>");
    client.println("<option value=02>2</option>");
    client.println("<option value=03>3</option>");
    client.println("<option value=04>4</option>");
    client.println("<option value=05>5</option>");
    client.println("<option value=06>6</option>");
    client.println("<option value=07>7</option>");
    client.println("<option value=08>8</option>");
    client.println("<option value=09>9</option>");
    client.println("<option value=10>10</option>");
    client.println("<option value=11>11</option>");
    client.println("<option value=12>12</option>");
    client.println("</select>");
    
    client.println("<select name=\"D\">");
    client.println("<option value=-1>D</option>");
    client.println("<option value=01>1</option>");
    client.println("<option value=02>2</option>");
    client.println("<option value=03>3</option>");
    client.println("<option value=04>4</option>");
    client.println("<option value=05>5</option>");
    client.println("<option value=06>6</option>");
    client.println("<option value=07>7</option>");
    client.println("<option value=08>8</option>");
    client.println("<option value=09>9</option>");
    client.println("<option value=10>10</option>");
    client.println("<option value=11>11</option>");
    client.println("<option value=12>12</option>");
    client.println("<option value=13>13</option>");
    client.println("<option value=14>14</option>");
    client.println("<option value=15>15</option>");
    client.println("<option value=16>16</option>");
    client.println("<option value=17>17</option>");
    client.println("<option value=18>18</option>");
    client.println("<option value=19>19</option>");
    client.println("<option value=20>20</option>");
    client.println("<option value=21>21</option>");
    client.println("<option value=22>22</option>");
    client.println("<option value=23>23</option>");
    client.println("<option value=24>24</option>");
    client.println("<option value=25>25</option>");
    client.println("<option value=26>26</option>");
    client.println("<option value=27>27</option>");
    client.println("<option value=28>28</option>");
    client.println("<option value=29>29</option>");
    client.println("<option value=30>30</option>");
    client.println("<option value=31>31</option>");

    client.println("</select>");
    
    
    client.println("<select name=\"Y\">");
    client.println("<option value=-1>Y</option>");
    client.println("<option value=2015>2015</option>");
    client.println("<option value=2016>2016</option>");
    client.println("<option value=2017>2017</option>");
    client.println("<option value=2018>2018</option>");
    
    client.println("</select> <br> <br>");
    
    client.println("Time: <br>");
    
    client.println("<select name=\"H\">");
    client.println("<option value=-1>H</option>");
    client.println("<option value=01>1</option>");
    client.println("<option value=02>2</option>");
    client.println("<option value=03>3</option>");
    client.println("<option value=04>4</option>");
    client.println("<option value=05>5</option>");
    client.println("<option value=06>6</option>");
    client.println("<option value=07>7</option>");
    client.println("<option value=08>8</option>");
    client.println("<option value=09>9</option>");
    client.println("<option value=10>10</option>");
    client.println("<option value=11>11</option>");
    client.println("<option value=12>12</option>");
    client.println("</select>");
        
    client.println("<select name=\"M\">");
    client.println("<option value=-1>M</option>");
    client.println("<option value=00>0</option>");
    client.println("<option value=01>1</option>");
    client.println("<option value=02>2</option>");
    client.println("<option value=03>3</option>");
    client.println("<option value=04>4</option>");
    client.println("<option value=05>5</option>");
    client.println("<option value=06>6</option>");
    client.println("<option value=07>7</option>");
    client.println("<option value=08>8</option>");
    client.println("<option value=09>9</option>");
    client.println("<option value=10>10</option>");
    client.println("<option value=11>11</option>");
    client.println("<option value=12>12</option>");
    client.println("<option value=13>13</option>");
    client.println("<option value=14>14</option>");
    client.println("<option value=15>15</option>");
    client.println("<option value=16>16</option>");
    client.println("<option value=17>17</option>");
    client.println("<option value=18>18</option>");
    client.println("<option value=19>19</option>");
    client.println("<option value=20>20</option>");
    client.println("<option value=21>21</option>");
    client.println("<option value=22>22</option>");
    client.println("<option value=23>23</option>");
    client.println("<option value=24>24</option>");
    client.println("<option value=25>25</option>");
    client.println("<option value=26>26</option>");
    client.println("<option value=27>27</option>");
    client.println("<option value=28>28</option>");
    client.println("<option value=29>29</option>");
    client.println("<option value=30>30</option>");
    client.println("<option value=31>31</option>");
    client.println("<option value=32>32</option>");
    client.println("<option value=33>33</option>");
    client.println("<option value=34>34</option>");
    client.println("<option value=35>35</option>");
    client.println("<option value=36>36</option>");
    client.println("<option value=37>37</option>");
    client.println("<option value=38>38</option>");
    client.println("<option value=39>39</option>");
    client.println("<option value=40>40</option>");
    client.println("<option value=41>41</option>");
    client.println("<option value=42>42</option>");
    client.println("<option value=43>43</option>");
    client.println("<option value=44>44</option>");
    client.println("<option value=45>45</option>");
    client.println("<option value=46>46</option>");
    client.println("<option value=47>47</option>");
    client.println("<option value=48>48</option>");
    client.println("<option value=49>49</option>");
    client.println("<option value=50>50</option>");
    client.println("<option value=51>51</option>");
    client.println("<option value=52>52</option>");
    client.println("<option value=53>53</option>");
    client.println("<option value=54>54</option>");
    client.println("<option value=55>55</option>");
    client.println("<option value=56>56</option>");
    client.println("<option value=57>57</option>");
    client.println("<option value=58>58</option>");
    client.println("<option value=59>59</option>");

    client.println("</select>");

    client.println("<input type=\"radio\" name=\"ampm\" value=\"am\" checked>Am");
    client.println("<input type=\"radio\" name=\"ampm\" value=\"pm\">Pm <br> <br>");


    client.println("Set Reccurance: <br>");
    client.println("<input type=\"radio\" name=\"r\" value=\"n\" checked>None");
    client.println("<input type=\"radio\" name=\"r\" value=\"d\">Daily");
    client.println("<input type=\"radio\" name=\"r\" value=\"w\">Weekly");
    client.println("<input type=\"radio\" name=\"r\" value=\"m\">Monthly");
    client.println("<br> <br>");


    client.println("Set Switches: Select only one in each row <br>");
    client.println("Switch 1: On <input type=\"checkbox\" name=\"Z\" value=\"A\"> Off <input type=\"checkbox\" name=\"z\" value=\"a\">");
    client.println("<br>");
    client.println("Switch 2: On <input type=\"checkbox\" name=\"Y\" value=\"B\"> Off <input type=\"checkbox\" name=\"y\" value=\"b\">");
    client.println("<br>");
    client.println("Switch 3: On <input type=\"checkbox\" name=\"X\" value=\"C\"> Off <input type=\"checkbox\" name=\"x\" value=\"c\">");
    client.println("<br>");
    client.println("Switch 4: On <input type=\"checkbox\" name=\"W\" value=\"D\"> Off <input type=\"checkbox\" name=\"w\" value=\"d\">");
    client.println("<br>");
    client.println("Switch 5: On <input type=\"checkbox\" name=\"V\" value=\"E\"> Off <input type=\"checkbox\" name=\"v\" value=\"e\">");

    client.println("<br> <br>");

    client.println("<input type=\"submit\" value=\"Submit\" >");
    client.println("</div>");
    client.println("</form>");
    
    client.println("<div align =\"center\">");
    //updateLog(0);
    //updateLog(1);
    int storage;
    for (int i = 0; i < 10; i++)
		{
			storage = ((Dcounter-i) % 10);
			convertEpoch2Structure(smokeLog[((Dcounter-i) % 10)], temp);
			client.println("Door Open ");
			client.println(i);
			client.println(": ");
			client.println(temp.tm_hour);
			client.println(":");
			client.println(temp.tm_min);
			client.println(":");
			client.println(temp.tm_sec);
			client.println(" ");
			client.println((temp.tm_mon+1));
			client.println("/");
			client.println(temp.tm_mday);
			client.println("/");
			client.println((temp.tm_year+1900));
			client.println("<br>");
		}
    client.println("<br>");
    for (int i = 0; i < 10; i++)
		{
			storage = ((Scounter-i) % 10);
			convertEpoch2Structure(smokeLog[((Scounter-i) % 10)], temp);
			client.println("Smoke Alarm ");
			client.println(i);
			client.println(": ");
			client.println(temp.tm_hour);
			client.println(":");
			client.println(temp.tm_min);
			client.println(":");
			client.println(temp.tm_sec);
			client.println(" ");
			client.println((temp.tm_mon+1));
			client.println("/");
			client.println(temp.tm_mday);
			client.println("/");
			client.println((temp.tm_year+1900));
			client.println("<br>");
		}
    
    client.println("</div>");
    // The HTTP response ends with another blank line:
    client.println("</body>");
}


void url_parse(char* buffer){
    tm tempTime;
    time_t tempEpoch = 0;
    z = 0;
    int j;
    valid = 1;
    
    int count = 20;
    int recurD = 0;
    int recurW = 0;
    int recurM = 0;
    
    while(buffer[z] != 'M')
    {
    	z++;
    }
    
    if (valid && buffer[z+2] != '-')
    {
    	month = (((int) buffer[z+2]) - 48) * 10 + ((int) buffer[z+3]) - 48;
    }
    else 
    {
    	valid = 0;
    }
    z = z + 5;
    if (valid && buffer[z+2] != '-')
    {
    	day = (((int) buffer[z+2]) - 48) * 10 + ((int) buffer[z+3]) - 48;
    }
    else 
    {
    	valid = 0;
    }
    z = z+5;
    if (valid && buffer[z+2] != '-')
    {
    	year = (((int) buffer[z+2]) - 48) * 1000 + (((int) buffer[z+3]) - 48)* 100 + (((int) buffer[z+4]) - 48)* 10 + ((int) buffer[z+5]) - 48;
    }
    else 
    {
    	valid = 0;
    }
    z = z + 7;
    
    if (valid && buffer[z+2] != '-')
    {
    	hours = (((int) buffer[z+2]) - 48)* 10 + ((int) buffer[z+3]) - 48;
    }
    else 
    {
    	valid = 0;
    }
    z = z + 5;
    
    if (valid && buffer[z+2] != '-')
    {
    	minutes = (((int) buffer[z+2]) - 48)* 10 + ((int) buffer[z+3]) - 48;
    }
    else 
    {
    	valid = 0;
    }
    z = z + 8;
    
    if (valid && buffer[z+2] == 'p')
    {
    	hours = hours + 12;
    }
    z = z + 5;
    
    if (valid)
    {
    	recur = buffer[z+2];	
    }
    
    z=z+3;
    
    action = 0;
    
    if(buffer[z]==NULL){
        valid = 0;
    }
    else{
        for(j=z; buffer[j]!=NULL; j++){
            //s1off
            if(buffer[j]=='a'){
                action += 512;
            }
            //s1on
            if(buffer[j]=='A'){
                action += 256;
            }
            //s2off
            if(buffer[j]=='b'){
                action += 128;
            }
            //s2on
            if(buffer[j]=='B'){
                action += 64;
            }
            //s3off
            if(buffer[j]=='c'){
                action += 32;
            }
            //s3on
            if(buffer[j]=='C'){
                action += 16;
            }
            //s4off
            if(buffer[j]=='d'){
                action += 8;
            }
            //s4on
            if(buffer[j]=='D'){
                action += 4;
            }
            //s5off
            if(buffer[j]=='e'){
                action += 2;
            }
            //s5on
            if(buffer[j]=='E'){
                action += 1;
            }
            else{
                valid = 0;
            }
        }
    }
    
    for (int i = 0; i < count; ++i)
    {
    	tempTime.tm_min = minutes;
        tempTime.tm_hour = hours;
        tempTime.tm_mday = day;
        tempTime.tm_mon = month - 1;
        tempTime.tm_year = year - 1900;
    
    	convertStructure2Epoch(tempTime, tempEpoch);
    	scheduleEvent(tempEpoch, action);
    	
    	if(recur=='d'){
    	    day +=1;
    	}
    	else if(recur=='w'){
    	    day +=7;
    	}
    	else if(recur=='m'){
    	    month +=1;
    	}
    	else 
    	{
    	    count = 0;
    	}
    }
    count = 10;
}

int scheduleEvent(time_t epoch, time_t action)
{
	int flag = 1;
	for (int i = 0; i < 256; i = i+2)
	{
		if (flag && schedule[i] == 0)
		{
			schedule[i] = epoch;
			schedule[i+1] = action;
			break;
			flag  = 0;
		}
	}
	return flag;
}

int checkSchedule(time_t epoch)
{
	for (int i = 0; i < 256; i = i+2)
	{
		if (schedule [i] - epoch >= 0 && schedule[i] - epoch < 600)
		{
			schedule [i] = 0;
			return schedule[i+1];
		}
	}
	return -1;
}