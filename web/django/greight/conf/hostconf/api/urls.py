from django.urls import include,path

urlpatterns = [
    path("", include('app.api.urls'))
]