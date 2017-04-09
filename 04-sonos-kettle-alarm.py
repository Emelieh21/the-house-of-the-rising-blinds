import requests
import json
import time
import datetime
import urllib2

def passed_time(initial_time):
	return datetime.timedelta.total_seconds(datetime.datetime.now() - initial_time) * 1000

headers = {'content-type': 'application/json', 'Authorization': '<YOUR_RELAYR_TOKEN>', 'Cache-Control':'no-cache'}

while True:
	r = requests.get('https://api.relayr.io/devices/<DEVICE_ID_OF_ALEXA_DEVICE>/readings', headers=headers)
	response = json.loads(r.text)
	#print response
	
	try:
		print response['readings'][0]['value']
		alexaStatus = response['readings'][0]['value']
		if alexaStatus == 260:
			alarmTime = "07:30:00"
		elif alexaStatus == 350:
			alarmTime = "07:40:00"
		elif alexaStatus == 400:
			alarmTime = "07:50:00"
		else:
			alarmTime = "never"
	except:
		print "response failed."
		alarmTime = "never"	
	

	timez = str(datetime.datetime.now().strftime("%H:%M:%S"))
	print timez
	if timez == alarmTime:
		try:
			response = urllib2.urlopen('http://<IP_OF_SONOS_SERVER>:5005/<ROOM>/play')
			html = response.read()
			print html
		except:
			print "fail"
		data = {'meaning': 'kettle', 'value': 'true'}
		r = requests.post('https://api.relayr.io/devices/<DEVICE_ID_OF_KETTLE_DEVICE>/data', data=json.dumps(data), headers=headers)
	
	time.sleep(0.5)