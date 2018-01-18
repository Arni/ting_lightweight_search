<?php 

function process_availability($results) {
  //"ding_availability":{"availability-77300090009869-bog":["90009869"],"availability-77300090430505-bog":["90430505","45226492"],"availability-77300024346196-bog":["24346196","25102886","41464887"],"availability-77300043835440-bog":["43835440"],"availability-77300042017078-bog":["42017078"],"availability-77300043393588-bog":["43393588"],"availability-77300024869547-bog":["24869547"],"availability-77300022630377-bog":["22630377"]}
  $availability_settings = array();
  foreach ($results as $result) {
    foreach ($result['types'] as $type) {
      if (!empty($type['object_ids'])) {
        foreach ($type['object_ids'] as $object_id) {
          $availability_settings[availability_id($type)][] =  $object_id;
        }
      }
    }
  }
  return array("ding_availability" => $availability_settings);
}

function availability_text($type) {
  $available_online_label = (is_available_online ($type)) ? '<span class="availability-status">online</span>' : '';
  return $type . $available_online_label;
}

function availability_id ($type) {
  $id = 'availability-' . $type['owner_id'] . $type['local_id'] . '-' .  strtolower($type['type']);
  $id = preg_replace('/[^A-Za-z0-9\-_]/', '', $id);
  return $id;
}

/**
 * Default online types.
 *
 * Defined as a function as a define can only be scalars.
 */
function default_online_types() {
  return array(
    'musik (net)',
    'ebog',
    'netdokument',
    'lydbog (net)',
    'film (net)',
    'tidsskrift (net)',
    'periodikum (net)',
    'pc-spil (net)',
    'avis (net)',
  );
}

function is_available_online ($type) {
  $online_types = default_online_types();
  return in_array(strtolower($type), $online_types);
}

function availability_class ($type) {
  // Mark as available if it is an online resource
  $available_online = (is_available_online ($type)) ? 'available' : 'not-available-online';
  return 'availability ' . $type . ' pending ' . $available_online;
}

function is_check_availability_type ($ac_source, $type) {
  $lib_materials_types = array('Bibliotekets materialer', 'Bibliotekskatalog');
  $library_material = in_array($ac_source, $lib_materials_types);
  return $library_material && !is_available_online ($type); 
}

function is_same_type ($type1, $type2, $id) {
  return trim($type1) == trim($type2);
}

?>