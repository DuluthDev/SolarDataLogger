$PORT='COM7'  
$BAUDRATE=9600  
$Parity=[System.IO.Ports.Parity]::None # System.IO.Ports.Parity  
$dataBits=8  
$StopBits=[System.IO.Ports.StopBits]::one # System.IO.Ports.StopBits  

$period = [timespan]::FromSeconds($DelaySeconds)  
$port= new-Object System.IO.Ports.SerialPort $PORT,$BAUDRATE,$Parity,$dataBits,$StopBits  
$port.Open()  

# Gets the data from the com port for the specified interval  
while (true) {   
  $message=$port.ReadLine()  
  echo $message >> .\output.txt
  Write-Output $message  
  DelaySeconds(1)
}
