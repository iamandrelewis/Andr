from django.http import HttpResponseRedirect
from django.conf import settings
DEFAULT_URL_REDIRECT = getattr (settings, "DEFAULT_URL_REDIRECT", "http://www.localhost:8000")

def wildcard_redirect(request, path=None):
    newUrl = DEFAULT_URL_REDIRECT
    if(path is not None):
        newUrl = DEFAULT_URL_REDIRECT + "/" + path
    return HttpResponseRedirect(newUrl)