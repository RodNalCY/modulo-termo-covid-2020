<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use GuzzleHttp\Client;

class PostsController extends Controller
{

    //https://jsonplaceholder.typicode.com/
    //http://docs.guzzlephp.org/en/stable/quickstart.html
    public function index(){
        $client = new Client([
            // Base URI is used with relative requests
            'base_uri' => 'http://172.168.100.105',
            'timeout'  => 2.0,
        ]);

        // Send a request to https://foo.com/api/test
        $response = $client->request('GET', 'temperature/1');
        //$posts = json_decode( $response->getBody()->getContents());
        $posts =  $response->getBody()->getContents();
        //return $posts;
        return view('posts.index', compact("posts"));
    }

    public function callAPITemperatura(Request $request){
        if ($request->ajax()) {

            $client = new Client([
                'base_uri' => 'http://172.168.100.105',
                'timeout'  => 2.0,
            ]);
            $response = $client->request('GET', 'temperature/1');
            $posts = $response->getBody()->getContents();
            return $posts;
           //return response()->json($lista_usuarios);
         }


    }
}
