import server from 'server';

const { get } = server.router;

server({
    public: '../html_data',
},[get('/', '')]);
console.log('Listen on http://localhost:3000/')