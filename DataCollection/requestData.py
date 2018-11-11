#python request map data code
import numpy as np
import time
#import http.client
import urllib.request
import urllib.parse
import json

startLat = 53.501308
endLat =   53.301882
startLong = -1.789030
endLong =    -1.316797
granularityLat =  16
granularityLong = 23
#horizontal dist = 31.23km or 1.3578km/block
#vertical dist = 22.18km or 1.38626km/block


latRes = (endLat-startLat)/granularityLat
longRes = (endLong-startLong)/granularityLong

latitudes = np.linspace(startLat,endLat, granularityLat)
longitudes = np.linspace(startLong,endLong, granularityLong)
#
#print(latitudes)
#print(longitudes)

#latitudes = startLat:latRes:endLat
#longitudes = startLong:longRes:endLong
urlA = 'https://maps.googleapis.com/maps/api/elevation/json?locations='
urlB = ','
urlC = '&key=AIzaSyDvoKjMeNfsmFiUQDPrPjtTlofuFRTGtYY'

if 1==0:
    urlStr = 'https://maps.googleapis.com/maps/api/elevation/json?locations=53.371815,-1.557013&key=AIzaSyDvoKjMeNfsmFiUQDPrPjtTlofuFRTGtYY'

    f = urllib.request.urlopen(urlStr)
    result = f.read().decode('utf-8')
    print(result)
    print(type(result))
    resultJSON = json.loads(result)
    print(resultJSON)
    result2 = resultJSON['results'][0]
    print(result2)
    elevation = result2['elevation']
    resolution = result2['resolution']
    print(elevation)
    print(resolution)

with open('resolutions.txt', 'w') as rf:
    with open("elevations.txt", "w") as ef:
        for x in range(granularityLong+1):
            ef.write(str(chr(ord('A')+x)))
            rf.write(str(chr(ord('A')+x)))
            ef.write(',')
            rf.write(',')
        ef.write('\n')
        rf.write('\n')
        currentLat = 0
        for lat in latitudes:
            print("Current Lat = " + str(currentLat) + '\n')
            currentLat = currentLat + 1
            for long in longitudes:
                urlStr = urlA + str(lat) + urlB + str(long) + urlC
                f = urllib.request.urlopen(urlStr)
                result = f.read().decode('utf-8')
                resultJSON = json.loads(result)
                result2 = resultJSON['results'][0]
                elevation = result2['elevation']
                ef.write(str(int(elevation))+',')
                resolution = result2['resolution']
                rf.write(str(int(resolution))+',')
                time.sleep(0.5)
            ef.write('\n')
            rf.write('\n')
