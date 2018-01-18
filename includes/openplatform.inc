<?php


function open_platform_theme($existing, $type, $theme, $path) {
  return array(
    'open_platform_covers' => array(
      'variables' => array('results' => NULL),
      'template'  => 'covers-search-result',
      'path' => $path . '/templates',
    ),
  );
}


function open_platform_search($query) {
  $parameters = array(
    "fields" => array(
      "title",
      "creator",
      "pid",
     // 'coverUrlFull',
    ),
    "pretty" => true,
    "timings" => true,
    "q" => $query,
    "offset" => 0,
    "limit" => 20,
    "sort" => "date_descending",
  );
  return open_platform_request('search', $parameters);
}

function open_platform_search_collection($query) {
  $parameters = array(
    "fields" => array(
      "title",
      "collection",
      "creator",
      "pid",
    ),
    "pretty" => true,
    "q" => $query,
    "offset" => 0,
    "limit" => 15,
  );
  return open_platform_request('search', $parameters);
}

function open_platform_search_series($query, $offset = 0) {
  $parameters = array(
    "fields" => array(
      "title",
      "collection",
      "creator",
      "pid",
      "titleSeries",
    ),
    "pretty" => true,
    "q" => $query,
    "offset" => $offset,
    "limit" => 20,
  );
  return open_platform_request('search', $parameters);
}

function open_platform_recommender($ids, $number_of_objects = 20) {
  $parameters = array(
    "fields" => array(
      "title",
      "creator",
      "pid",
    ),
    "pretty" => true,
    "like" => $ids,
    "recommender" => 'default',
    "limit" => $number_of_objects,
  );
  return open_platform_request('recommend', $parameters);
}

function open_platform_render_cover($data) {
  $results = array();
  foreach ($data as $object) {
    if (isset($object->coverUrlFull)) {
      $image = '<img src="' . $object->coverUrlFull[0]  . '" width="140" height="203" styling="height:203px">';
      $results[] = array(
      'snippet' => $image 
      );
    }  
  }  
  $content = theme('open_platform_covers', array('results' => $results)); 
  return $content;
}


function open_platform_get_token() {
  $key = "1ba2428f-1fc3-4159-82ff-869b87026bcd";
  $secret = "3ab379e23032f8e94599ba2a75be13a4d49e846342d101a6899b27b823abff1e";

// request token
  $basic_credentials = base64_encode($key.':'.$secret);
  $tk = curl_init('https://auth.dbc.dk/oauth/token');
  curl_setopt($tk, CURLOPT_HTTPHEADER, array('Authorization: Basic '.$basic_credentials, 'Content-Type: application/x-www-form-urlencoded;charset=UTF-8'));
  curl_setopt($tk, CURLOPT_POSTFIELDS, 'grant_type=password&username=@773000&password=@773000');
  curl_setopt($tk, CURLOPT_RETURNTRANSFER, true);
  $response = curl_exec($tk);
  curl_close($tk);
  file_put_contents('/var/www/drupalvm/drupal/debug/opentoken1.txt', $response, FILE_APPEND);
  $token = json_decode('{"access_token": "2c0860a00b6fd42525cd68b8cec3121fa125cbe6"}');
  file_put_contents('/var/www/drupalvm/drupal/debug/opentoken2.txt', $token, FILE_APPEND);
  if (isset($response['access_token'])) {
    return $response['access_token'];
  }
}

function open_platform_request($operation, $parameters) {
  //$token = variable_get('ting_open_platform_token', 'ab32da0a2e25d8b720e6af316578ea8e9fa120a8');
  $parameters['access_token'] = '0ad1cf8703d09f20ff356fc52852ff752c990b50';
  $service_url = 'https://openplatform.dbc.dk/v1/' . $operation;
  
  $curl = curl_init($service_url);
  curl_setopt($curl, CURLOPT_HTTPHEADER, array(                                                                          
    'Content-Type: application/json',                                                                                
    'Accept: application/json')                                                                       
  ); 
  curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
  curl_setopt($curl, CURLOPT_POST, true);
  curl_setopt($curl, CURLOPT_POSTFIELDS, json_encode($parameters));
  $curl_response = curl_exec($curl);
  if ($curl_response === false) {
    $info = curl_getinfo($curl);
    curl_close($curl);
  }
  curl_close($curl);
  $decoded = json_decode($curl_response);
  if (isset($decoded->response->status) && $decoded->response->status == 'ERROR') {
    //TODO Handle error
  }
  return  $decoded;
}



