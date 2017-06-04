#!/usr/bin/flask python
# -*- coding: utf-8 -*-

import os
from ctex import server

if __name__ == '__main__':
    port = int(os.environ.get('PORT', 5001))
    server.app.run(host='0.0.0.0', port=port)