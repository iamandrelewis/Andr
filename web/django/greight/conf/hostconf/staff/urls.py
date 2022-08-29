from django.urls import include,path

urlpatterns = [
    path("", include('app.staff.urls'))
]