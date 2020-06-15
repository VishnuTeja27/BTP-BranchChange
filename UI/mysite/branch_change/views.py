from django.shortcuts import render
from django.http import HttpResponse
from django.views.decorators.csrf import csrf_exempt
import subprocess
import os
from django.contrib.staticfiles import finders
from django.core.files.storage import FileSystemStorage
# import commands


def index1(request):
    # return HttpResponse("Hello, world. You're at the polls index.")
    return render(request, 'branch_change/index.html')
# Create your views here.
@csrf_exempt
def index(request):
	# template='index.html'
    if request.method =='GET':
        return render(request,'branch_change/index.html')
    file=request.FILES['file']
    fs = FileSystemStorage()
    filename = fs.save("input.txt", file)
    # result = finders.find('css/base.css')
    # searched_locations = finders.searched_locations
    # status, res = commands.getstatusoutput("/home/vishnuteja/Desktop/a.out")
    # process12=subprocess.call(["cd" ,"media"])
    # process=subprocess.call(["g++","-std=c++11","main.cpp"])
    process1=subprocess.call(["./a.out","input.txt"])

    os.remove("input.txt")
    # output = process.stdout
    # run(["python earnings", "symbol"], stdout=subprocess.PIPE)
    # subprocess.call(["gcc","/home/vishnuteja/Desktop/ab.c","-o","nairobi"])
    # subprocess.call(["./nairobi"])
    # if not file.name.endswith('.csv'):
    #     messages.error(request,'This is not a csv file')
    return render(request,'branch_change/index.html',{'res':process1})
