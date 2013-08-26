<?php

  function getTitle($object) {
    // If we couldn't find the object, use the title of the first object.
    $title = FALSE;
    if (isset($object->record['dc:title'])) {
      if (isset($object->record['dc:title']['dkdcplus:full'])) {
        $title = $object->record['dc:title']['dkdcplus:full'][0];
      }
      else {
        $title = $object->record['dc:title'][''][0];
      }
    }
    return $title;
  }
  
  function getObjectType($object) {
    return !empty($object->record['dc:type']['dkdcplus:BibDK-Type'][0]) ? $object->record['dc:type']['dkdcplus:BibDK-Type'][0] : FALSE;
  }
  
  function getAbstract($object) {
    return !empty($object->record['dcterms:abstract'][''][0]) ? $object->record['dcterms:abstract'][''][0] : FALSE;
  }
  
   function getCreators($object) {
    $creators = array();
    if (!empty($object->record['dc:creator'])) {
      foreach ($object->record['dc:creator'] as $type => $dc_creator) {
        if ($type != 'oss:sort') {
          $creators = array_merge($creators, $dc_creator);
        }
      }
    }
    return $creators;
  }

  function getSubjects($object) {
    $subjects = array();
    if (!empty($object->record['dc:subject'])) {
      foreach ($object->record['dc:subject'] as $type => $dc_subject) {
        if (!in_array($type, array('dkdcplus:genre', 'dkdcplus:DK5', 'dkdcplus:DK5-Text', 'dkdcplus:DBCO', 'dkdcplus:DBCN'))) {
          $subjects = array_merge($subjects, $dc_subject);
        }
      }
    }
    return $subjects;
  }
  
  function getLocalId($object) {
    return !empty($object->localId) ? $object->localId : FALSE;
  }

  function getOwnerId($object) {
    return !empty($object->ownerId) ? $object->ownerId : FALSE;
  }
  
  ?>