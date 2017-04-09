import requests
import json
import time
import datetime
import urllib2

def passed_time(initial_time):
	return datetime.timedelta.total_seconds(datetime.datetime.now() - initial_time) * 1000

flag = False
# Initialize the GPIO 2.
# Set the initialized GPIO as an output.

headers = {'content-type': 'application/json', 'Authorization': '<YOUR_RELAYR_TOKEN>', 'Cache-Control':'no-cache'}

while True:
	r = requests.get('https://api.relayr.io/devices/<DEVICE_ID>/readings', headers=headers)
	response = json.loads(r.text)
	#print response
	
	try:
		print response['readings'][0]['value']
		alexaStatus = response['readings'][0]['value']
	except:
		print "response failed."
		alexaStatus = 2000
	
	try:
		print response['readings'][1]['value']
		sunStatus = response['readings'][1]['value']
	except:
		print "response failed."
		sunStatus = 100		
	
	if flag == False:
		if sunStatus < 50:
			flag = True
			response = urllib2.urlopen('http://<SONOS_SERVER_IP>:5005/<ROOM>/pause')
			try:
				html = response.read()
				print html
			except:
				print "fail"
	if flag == True:
		d = datetime.datetime.now()
		#print d.hour
		if d.hour > 7:
			if sunStatus > alexaStatus:
				flag = False
				response = urllib2.urlopen('http://<SONOS_SERVER_IP>:5005/<ROOM>/play')
				try:
					html = response.read()
					print html
				except:
					print "fail"
				data = {'meaning': 'kettle', 'value': 'true'}
				r = requests.post('https://api.relayr.io/devices/<DEVICE_ID>/data', data=json.dumps(data), headers=headers)
		
	time.sleep(0.5)