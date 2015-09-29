import web
from driver import Driver
import json
import sys

urls = (
    '/', 'index',
    '/test.js', 'testjs',
    '/driver', 'driver'
)


DEVICE = '/dev/ttyACM2'


class testjs:
    def GET(self):
        return open('./test.js','r').read()


class index:
    def GET(self):
        hello = web.template.frender('test.html')
        return hello()  
        #return "Pump Control v0.1"


class driver:
    def POST(self):
        web.header('Content-Type', 'application/json')
        i = web.input()
        command = i.cmd
        try:
            if command == 'run':
                params = i.param
                p = params.split(' ')
                web.board.run(int(p[0]), float(p[1]))
            elif command == 'status':
                return  json.dumps({"status": web.board.checkStatus()})
            elif command == 'stop':
                web.board.stop()
            elif command == 'halt':
                web.board.halt()
        except Exception,msg:
            print "Error"
            print msg
            return {'error' : ' %s' % msg}
        return {'result' : 'ok'}

    def GET(self):
        return 'HELLO!'



if __name__ == "__main__":


    board = Driver(DEVICE)
    web.board = board
    app = web.application(urls, globals())
    app.run()
