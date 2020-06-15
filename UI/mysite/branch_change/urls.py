from django.urls import path

from . import views
from django.conf.urls import *
urlpatterns = [
    path('', views.index, name='index'),
]
