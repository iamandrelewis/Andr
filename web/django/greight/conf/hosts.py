from django.conf import settings
from django_hosts import patterns, host

host_patterns = patterns('',

    host(r'www',settings.ROOT_URLCONF,name='default'),
    host(r'admin','conf.hostconf.admin.urls',name='admin'),
    host(r'staff','conf.hostconf.staff.urls',name='staff'),
    host(r'beta','conf.hostconf.beta.urls', name='beta'),
    host(r'dev','conf.hostconf.dev.urls',name='dev'),
    host(r'api','conf.hostconf.api.urls',name='api'),
    host(r'shop','conf.hostconf.shop.urls',name='shop'),
    host(r'(?!www).*','conf.hostconf.redirect.urls', name='wildcard'),
)