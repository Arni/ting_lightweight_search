<?php

/**
 * @file
 * Attempt to deliver a cached version of a page depending on the users role.
 *
 * This file gets included by _drupal_bootstrap_page_cache in bootstrap.inc
 * during the drupal bootstrap stage DRUPAL_BOOTSTRAP_PAGE_CACHE. This script
 * delegates the request to the underlying cache handler and attempts to deliver
 * a cached version of a page for this request.
 *
 * In the event of a cache-miss or if a page is not cachable, execution is
 * passed back to _drupal_bootstrap_page_cache and the page will be served
 * using a full bootstrap.
 *
 * @see _drupal_bootstrap_page_cache()
 */

$delivered = ting_lightweight_search_builtin_cacheinc_retrieve_cache_page();
if ($delivered) {
  exit;
}

/**
 * Check whether client is allowed to skip the cache for this request.
 *
 * The default implementation allows cache bypass for all clients having a
 * session cookie.
 *
 * @see _ting_lightweight_search_default_nocache_set()
 */
function _authcache_builtin_cacheinc_default_nocache_get() {
  if (isset($_COOKIE['nocache']) && isset($_COOKIE[session_name()])) {
    return TRUE;
  }
}

/**
 * Retrieve the authcache key for a session from the key-cache.
 */
function _ting_lightweight_search_builtin_cacheinc_cache_key_get($session_id) {
  $cache = cache_get($session_id, 'cache_ting_lightweight_search_key');

  if ($cache === FALSE || $cache->expire > 0 && $cache->expire < REQUEST_TIME) {
    // Need a full bootstrap when key was not found or entry expired.
    return FALSE;
  }
  else {
    return $cache->data;
  }
}

/**
 * Retrieve the authcache key for a session from a cookie.
 */
function _ting_lightweight_search_builtin_cacheinc_cookie_key_get($session_id) {
  if (!isset($_COOKIE['authcache'])) {
    return FALSE;
  }
  else {
    return $_COOKIE['authcache'];
  }
}

/**
 * Add headers required to make stupid browsers behave.
 *
 * When modifying this function, do not forget to adapt the respective VCL
 * snipped shipping with the Authcache Varnish Backend.
 */
function _authcache_builtin_cacheinc_fix_browsers() {
  // Disable browser caching in Safari
  //
  // @see:
  // - https://bugs.webkit.org/show_bug.cgi?id=71509
  // - https://groups.drupal.org/node/191453
  // - https://drupal.org/node/1910178
  if (!empty($_SERVER['HTTP_USER_AGENT']) && strpos($_SERVER['HTTP_USER_AGENT'], 'Safari') !== FALSE && strpos($_SERVER['HTTP_USER_AGENT'], 'Chrome') === FALSE) {
    drupal_add_http_header('Cache-Control', 'no-cache, must-revalidate, post-check=0, pre-check=0');
  }
}

/**
 * Send cached page to browser, if found.
 *
 * @return bool
 *   TRUE if page was delivered, FALSE otherwise
 */
function ting_lightweight_search_builtin_cacheinc_retrieve_cache_page() {
  global $base_root;

  // Connect to database and initialize configuration if not disabled from
  // within settings.php
//  if (!variable_get('ting_lightweight_search_builtin_cache_without_database')) {

  drupal_bootstrap(DRUPAL_BOOTSTRAP_VARIABLES, FALSE);
//  }

  // The following three basic exclusion rules are mirrored in
  // ting_lightweight_search_authcache_request_exclude() in authcache.module
  // BEGIN: basic exclusion rules.
//  $vary_header = variable_get('authcache_builtin_vary', 'Cookie');
//  if (!function_exists('authcache_backend_init') || !authcache_backend_init('authcache_builtin', $vary_header)) {
//    return FALSE;
//  }

  // Only GET and HEAD requests allowed.
//  if (!($_SERVER['REQUEST_METHOD'] == 'GET' || $_SERVER['REQUEST_METHOD'] == 'HEAD')) {
//    return FALSE;
//  }


  // Try to retrieve a key when the session cookie is present on the request.
//  $key = NULL;
//  if (isset($_COOKIE[session_name()])) {
//    $session_id = $_COOKIE[session_name()];
//    switch (variable_get('authcache_key_method', 'cache')) {
//      case 'cache':
//        $key = _ting_lightweight_search_builtin_cacheinc_cache_key_get($session_id);
//        break;
//
//      case 'cookie':
//        $key = _ting_lightweight_search_builtin_cacheinc_cookie_key_get($session_id);
//        break;
//
//      default:
//        $key_get = variable_get('ting_lightweight_search_builtin_key_get');
//        if (!is_callable($key_get)) {
//          return FALSE;
//        }
//
//        $key = call_user_func($key_get, $session_id);
//        break;
//    }
//
//    // When key needs to be regenerated, return and trigger a full bootstrap.
//    if ($key === FALSE) {
//      return FALSE;
//    }
//  }

  // Fallback to anonymous content.
//  if (!$key) {
//    $generator = variable_get('authcache_key_generator');
//    if (is_callable($generator)) {
//      $key = call_user_func($generator);
//    }
//    else {
//      $key = $base_root;
//    }
//  }
//
//  // Attempt to retrieve page from cache.
//  $cid = $key . request_uri();
//  $cache = cache_get($cid, 'cache_page');
//
//  if (empty($cache)) {
//    header('X-Drupal-Cache: MISS');
//    return FALSE;
//  }
 // else {
    // Render cache benchmark.
    if (isset($_COOKIE['cache_render'])) {
      setcookie('cache_render', timer_read('page'), 0, ini_get('session.cookie_path'), ini_get('session.cookie_domain'), ini_get('session.cookie_secure') == '1');
    }
    $path = $_SERVER['REQUEST_URI'];
    if (substr( $path, 0, 12 ) === "/ting/search") {
    require_once DRUPAL_ROOT . '/profiles/ding2/modules/ting_lightweight_search/index.php';
    $query = substr( $path, 12);
    $search = search($query);
    $cache = (object) array(
    'data' => array(
      'body' => '<html>Test ' . $search  .'</html>',
      'headers' => array(),
    ),
    'created' => $created,
  );

    header('X-Drupal-Cache: HIT');
    _authcache_builtin_cacheinc_fix_browsers();
    ting_lightweight_search_serve_page_from_cache($cache, $key);
    return TRUE;
    } else {
      return FALSE;
    }
  //}
}


/**
 * @file
 * Defines authcache aware copy of drupal_serve_page_from_cache().
 */

/**
 * @defgroup authcache_backend Cache backend
 * @{
 */

/**
 * Sets HTTP headers in preparation for a cached page response.
 *
 * @see drupal_serve_page_from_cache()
 */
function ting_lightweight_search_serve_page_from_cache(stdClass $cache, $authcache_key) {
  // Negotiate whether to use compression.
  $page_compression = variable_get('page_compression', TRUE) && extension_loaded('zlib');
  $return_compressed = $page_compression && isset($_SERVER['HTTP_ACCEPT_ENCODING']) && strpos($_SERVER['HTTP_ACCEPT_ENCODING'], 'gzip') !== FALSE;

  // Get headers set in hook_boot(). Keys are lower-case.
  $hook_boot_headers = drupal_get_http_header();

  // Headers generated in this function, that may be replaced or unset using
  // drupal_add_http_headers(). Keys are mixed-case.
  $default_headers = array();

  foreach ($cache->data['headers'] as $name => $value) {
    // In the case of a 304 response, certain headers must be sent, and the
    // remaining may not (see RFC 2616, section 10.3.5). Do not override
    // headers set in hook_boot().
    $name_lower = strtolower($name);
    if (in_array($name_lower, array('content-location', 'expires', 'cache-control', 'vary')) && !isset($hook_boot_headers[$name_lower])) {
      drupal_add_http_header($name, $value);
      unset($cache->data['headers'][$name]);
    }
  }

  // Authcache modification: Unconditionally add Cache-Control: public.
  $default_headers['Cache-Control'] = 'public, max-age=' . variable_get('page_cache_maximum_age', 0);

  // Entity tag should change if the output changes.
  // Authcache modification: Add authcache key to entity tag.
  $key_tag = '';//!authcache_is_default_key($ting_lightweight_search_key) ? '-' . $authcache_key : '';
  $etag = '"' . $cache->created . '-' . intval($return_compressed) .  $key_tag . '"';
  header('Etag: ' . $etag);

  // See if the client has provided the required HTTP headers.
  $if_modified_since = isset($_SERVER['HTTP_IF_MODIFIED_SINCE']) ? strtotime($_SERVER['HTTP_IF_MODIFIED_SINCE']) : FALSE;
  $if_none_match = isset($_SERVER['HTTP_IF_NONE_MATCH']) ? stripslashes($_SERVER['HTTP_IF_NONE_MATCH']) : FALSE;

  if ($if_modified_since && $if_none_match
      && $if_none_match == $etag // etag must match
      && $if_modified_since == $cache->created) {  // if-modified-since must match
    header($_SERVER['SERVER_PROTOCOL'] . ' 304 Not Modified');
    drupal_send_headers($default_headers);
    return;
  }

  // Send the remaining headers.
  foreach ($cache->data['headers'] as $name => $value) {
    drupal_add_http_header($name, $value);
  }

  $default_headers['Last-Modified'] = gmdate(DATE_RFC1123, $cache->created);

  // HTTP/1.0 proxies does not support the Vary header, so prevent any caching
  // by sending an Expires date in the past. HTTP/1.1 clients ignores the
  // Expires header if a Cache-Control: max-age= directive is specified (see RFC
  // 2616, section 14.9.3).
  $default_headers['Expires'] = 'Sun, 19 Nov 1978 05:00:00 GMT';

  drupal_send_headers($default_headers);

  // Allow HTTP proxies to cache pages for anonymous users without a session
  // cookie. The Vary header is used to indicates the set of request-header
  // fields that fully determines whether a cache is permitted to use the
  // response to reply to a subsequent request for a given URL without
  // revalidation. If a Vary header has been set in hook_boot(), it is assumed
  // that the module knows how to cache the page.
  if (!isset($hook_boot_headers['vary']) && !variable_get('omit_vary_cookie')) {
    header('Vary: Cookie');
  }

//  if ($page_compression) {
//    header('Vary: Accept-Encoding', FALSE);
//    // If page_compression is enabled, the cache contains gzipped data.
//    if ($return_compressed) {
//      // $cache->data['body'] is already gzip'ed, so make sure
//      // zlib.output_compression does not compress it once more.
//      ini_set('zlib.output_compression', '0');
//      header('Content-Encoding: gzip');
//    }
//    else {
//      // The client does not support compression, so unzip the data in the
//      // cache. Strip the gzip header and run uncompress.
//      $cache->data['body'] = gzinflate(substr(substr($cache->data['body'], 10), 0, -8));
//    }
//  }

  // Print the page.
  print $cache->data['body'];
}

/**
 * Collect document and headers and pass them to cache backends.
 *
 * @return object
 *   A fake-cache object suitable for passing to drupal_serve_page_from_cache.
 *
 * @see drupal_page_set_cache()
 */
function authcache_backend_cache_save() {
  // Restore preferred header names based on the lower-case names returned
  // by drupal_get_http_header().
  $headers = array();
  $header_names = _drupal_set_preferred_header_name();
  foreach (drupal_get_http_header() as $name_lower => $value) {
    $headers[$header_names[$name_lower]] = $value;
  }

  // Retrieve and compress the document body.
  $body = ob_get_contents();
  if (variable_get('page_compression', TRUE) && extension_loaded('zlib')) {
    $body = gzencode($body, 9, FORCE_GZIP);
  }

  // Invoke cache backend.
  $created = REQUEST_TIME;
  module_invoke(ting_lightweight_search_backend(), 'authcache_backend_cache_save', $body, $headers, $created);

  // Remove buffer content.
  ob_clean();

  // Construct dummy cache object which is later served using
  // drupal_serve_page_from_cache.
  $cache = (object) array(
    'data' => array(
      'body' => $body,
      'headers' => $headers,
    ),
    'created' => $created,
  );

  return $cache;
}

/**
 * Initialize the cache backend module.
 */
function authcache_backend_init($module, $vary_header) {
  if (drupal_is_cli()) {
    return FALSE;
  }

  // Determine whether a cache backend already has been chosen.
  if (ting_lightweight_search_backend()) {
    return FALSE;
  }

  // Determine whether we can savely set the given vary header.
  if (!authcache_backend_check_vary($vary_header)) {
    return FALSE;
  }

  // Finally set vary header and cache backend.
  drupal_add_http_header('Vary', $vary_header);
  ting_lightweight_search_backend($module);
  return TRUE;
}

/**
 * Return the active cache backend module.
 */
function authcache_backend($set_cache_backend = NULL) {
  $cache_backend = &drupal_static(__FUNCTION__);

  if (isset($set_cache_backend)) {
    $cache_backend = $set_cache_backend;
  }

  return $cache_backend;
}

/**
 * Return true if the vary header is suitable for the active cache backend.
 */
function ting_lightweight_search_backend_check_vary($set_vary_header = NULL) {
  $vary_header = &drupal_static(__FUNCTION__);

  if (isset($set_vary_header)) {
    $vary_header = $set_vary_header;
  }

  $hook_boot_headers = drupal_get_http_header();
  if (isset($hook_boot_headers['vary']) && $hook_boot_headers['vary'] != $vary_header) {
    return FALSE;
  }
  else {
    return TRUE;
  }
}

/**
 * @} End of "defgroup authcache_backend"
 */

/**
 * @defgroup ting_lightweight_search_key Key computation and management
 * @{
 */

/**
 * Returns true if this request is using the default cache-key.
 */
function authcache_is_default_key($key) {
  global $base_root;

  // Check whether a key-generator for anonymous users is in place.
  $generator = variable_get('ting_lightweight_search_key_generator');
  if (is_callable($generator)) {
    return FALSE;
  }

  return $key == $base_root;
}

/**
 * @} End of "defgroup authcache_key"
 */

