<?php

/**
 * Callback for retrieving an specified institution.
 *
 * @return array
 */
function _institution_resource_retrieve($iid) {
  $institution = forschungsatlas_institution_load(intval($iid));
  $result = new stdClass();

  if ($institution) {
    $result->name = $institution['name'];
    $result->abbrev = empty($institution['abbrev']) ? null : $institution['abbrev'];
    $result->address = array(
        'street' => $institution['street'],
        'street2' => $institution['street2'],
        'postalcode' => $institution['postalcode'],
        'city' => $institution['city'],
        'federalstate' => $institution['federalstate'],
    );
    $result->url = empty($institution['url']) ? null : $institution['url'];
    $result->orgtype = $institution['orgtype'];
    $result->description = empty($institution['description']) ? null : $institution['description'];
    $result->changed = format_date($institution['changed'], 'custom', FORSCHUNGSATLAS_DATE_FORMAT);
    $result->geolocation = array(
        'geotype' => $institution['geotype'],
        'latitude' => $institution['latitude'],
        'longitude' => $institution['longitude'],
        'wkt' => $institution['wkt'],
    );
    // Mapping tables
    module_load_include('inc', 'forschungsatlas', 'includes/forschungsatlas.institution');
    $result->belonging = _forschungsatlas_get_belonging_list($iid);
    $result->network = _forschungsatlas_get_network_list($iid);
    $result->organisms = _forschungsatlas_get_organisms_list($iid);
    $result->habitats = _forschungsatlas_get_habitats_list($iid);
    $result->topics = _forschungsatlas_get_topics_list($iid);
    $result->intsubjects = _forschungsatlas_get_intsubjects_list($iid);
    $result->methodologies = _forschungsatlas_get_methodologies_list($iid);
    $result->subdivision = _forschungsatlas_get_subordinate_list($iid);
  }

  return $result;
}

/**
 * Callback for listing institutions
 *
 * @return array
 */
function _institution_resource_index($page) {
  $limit = FORSCHUNGSATLAS_PAGER;
  $offset = intval($page) * $limit;

  $result = db_query_range('SELECT iid, name, city, federalstate, assembledparentname
      FROM {forschungsatlas__aux_institutions}
      WHERE 1
      ORDER BY compositedname ASC',
      $offset, $limit);

  $rows = array();
  foreach ($result as $data) {
    $row = new stdClass();
    $row->iid = $data->iid;
    $row->name = $data->name;
    $row->city = $data->city;
    $row->federalstate = $data->federalstate;
    $row->assembledparentname = $data->assembledparentname;
    $rows[] = $row;
  }

  return $rows;
}
