## heartbeat_for_smart_factory
### 0. Paper
### 1. Heartbeat frame struct

<table border="1" width="500px" cellspacing="10">
<tr>
  <th align="center">Byte number</th>
  <th align="center">Byte quantity</th>
  <th align="center">Typical values</th>
  <th align="center">Meanings</th>
  <th align="center">Remarks</th>
</tr>
<tr>
  <td>0-6</td>
  <td>7</td>
  <td>0x57AB</td>
  <td>Lead code</td>
  <td rowspan="2" align="center">Frame Header 8 Bytes</td>
</tr>
<tr>
  <td>7</td>
  <td>1</td>
  <td>0xD5</td>
  <td>Delimiter</td>
</tr>
<tr>
  <td>8-13</td>
  <td>6</td>
  <td>0xFF FF FF FF FF FF</td>
  <td>Destination MAC address</td>
  <td rowspan="4" align="center">E II format 14 Bytes</td>
</tr>
<tr>
  <td>14-19</td>
  <td>6</td>
  <td>0xFF FF FF FF FF FF</td>
  <td>Source MAC address</td>
</tr>
<tr>
  <td>20</td>
  <td>1</td>
  <td>0x08</td>
  <td rowspan="2">Type IP 0x0800</td>
</tr>
<tr>
  <td>21</td>
  <td>1</td>
  <td>0x00</td>
</tr>
<tr>
  <td>22</td>
  <td>1</td>
  <td>0x45</td>
  <td>Protocol version</td>
  <td rowspan="10" align="center">IP header 20 Bytes</td>
</tr>
<tr>
  <td>23</td>
  <td>1</td>
  <td>0x00</td>
  <td>TOS(type of service)</td>
</tr>
<tr>
  <td>24-25</td>
  <td>2</td>
  <td>0x005C</td>
  <td>Total length(IP header + UDP header + data length) = 20 + 8 + 64 = 92(0x005C)</td>
</tr>
<tr>
  <td>26-27</td>
  <td>2</td>
  <td>0x0000</td>
  <td>Identification(autoincrement after sending a data packet)</td>
</tr> 
<tr>
  <td>28-29</td>
  <td>2</td>
  <td>0x0000</td>
  <td>Flags/offset</td>
</tr> 
<tr>
  <td>30</td>
  <td>1</td>
  <td>0x80</td>
  <td>TTL(Time to live)</td>
</tr> 
<tr>
  <td>31</td>
  <td>1</td>
  <td>0x11</td>
  <td>UDP protocol</td>
</tr> 
<tr>
  <td>32-33</td>
  <td>2</td>
  <td>0x00</td>
  <td>Header checksum</td>
</tr> 
<tr>
  <td>34-37</td>
  <td>4</td>
  <td>192_168_0_1</td>
  <td>Source IP address</td>
</tr> 
<tr>
  <td>38-41</td>
  <td>4</td>
  <td>0xFF FF FF FF</td>
  <td>Destination IP address</td>
</tr> 
<tr>
  <td>42-43</td>
  <td>2</td>
  <td>0x1F90(8080)</td>
  <td>Source Port</td>
  <td rowspan="4" align="center">UDP Header 8 Bytes</td>
</tr>
<tr>
  <td>44-45</td>
  <td>2</td>
  <td>0x1F91(8081)</td>
  <td>Destination Port</td>
</tr> 
<tr>
  <td>46-47</td>
  <td>2</td>
  <td>0x0048</td>
  <td>UDP header + data length = 8 + 64 = 72(0x0048)</td>
</tr> 
<tr>
  <td>48-49</td>
  <td>2</td>
  <td>0x00</td>
  <td>Header checksum</td>
</tr> 
<tr>
  <td>50-113</td>
  <td>1 PKG</td>
  <td>64 {0x00}</td>
  <td>Data</td>
  <td>1 PKG</td>
</tr>
<tr>
  <td>114-117</td>
  <td>4</td>
  <td>0x00</td>
  <td>CRC(Cyclic Redundancy Check)</td>
  <td>4 Bytes</td>
</tr>
</table>

### 2. Demo
`./demo` includes some demos for the project.		

