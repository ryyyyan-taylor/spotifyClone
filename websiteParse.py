from bs4 import BeautifulSoup
import requests

urls = []
urls.append("https://www.billboard.com/charts/hot-100")
urls.append("https://www.billboard.com/charts/hot-100/2019-04-06")
urls.append("https://www.billboard.com/charts/hot-100/2019-03-30")
urls.append("https://www.billboard.com/charts/hot-100/2019-03-23")
urls.append("https://www.billboard.com/charts/hot-100/2019-03-16")
urls.append("https://www.billboard.com/charts/hot-100/2019-03-09")
open("songs.txt", "w").close()
open("html.txt", "w").close()

for j in range (0,6):
	url = urls[j]
	html = requests.get(url)
	bsObj = BeautifulSoup(html.text, 'lxml')
	# h = open("html.txt", "a+")
	# h.write(str(bsObj))
	count = 1;
	songList = [];
	byList = [];
	f = open("songs.txt", "a+")

	titleList = bsObj.findAll("span",{"class":"chart-list-item__title-text"})

	for title in titleList:
		temp = title.text
		temp = temp.strip(' ')
		temp = temp.strip('\n')
		songList.append(temp)

	artistList = bsObj.findAll("div",{"class":"chart-list-item__artist"})

	for artist in artistList:
		temp = artist.text
		temp = temp.strip(' ')
		temp = temp.strip('\n')
		byList.append(temp)

	for i in range (0,100):
		song = songList[i] + "|" + byList[i]
		song = song.strip(' ')
		song = song.strip('\n')
		f.write(str(i+1) + "|" + song + "\n")

	f.write("\n")

f.close()