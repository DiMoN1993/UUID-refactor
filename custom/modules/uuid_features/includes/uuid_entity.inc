<?php
/**
 * Created by JetBrains PhpStorm.
 * User: developer
 * Date: 7/10/13
 * Time: 4:25 PM
 * To change this template use File | Settings | File Templates.
 */


/**
 * Implements hook_features_export_options().
 */
$entity_info = entity_get_info();
foreach ($entity_info as $entity_type => $type_info) {
  if (!isset($type_info['uuid'])) {
    continue;
  }
  if (!function_exists("uuid_{$entity_type}_features_export_options")) {
    $function_code = "function uuid_{$entity_type}_features_export_options() {return uuid_entity_features_export_options(\"$entity_type\");  }";
    eval($function_code);
  }
  if (!function_exists("uuid_{$entity_type}_features_export")) {
    $function_code = "function uuid_{$entity_type}_features_export(\$data, &\$export, \$module_name = '') {return uuid_entity_features_export(\"$entity_type\", \$data, \$export, \$module_name);  }";
    eval($function_code);
  }
  if (!function_exists("uuid_{$entity_type}_features_export_render")) {
    $function_code = "function uuid_{$entity_type}_features_export_render(\$module, \$data, \$module_name = '') {return uuid_entity_features_export_render(\"$entity_type\", \$module, \$data, \$module_name);  }";
    eval($function_code);
  }
}

function uuid_entity_features_export_options($entity_type) {
  $options = array();
  $type_info = entity_get_info($entity_type);
  $entities = entity_load($entity_type, FALSE);
  foreach ($entities as $entity) {
    $options[$entity->uuid] = t('@type @bundle: @title', array(
      '@type' => $entity_type,
      '@bundle' => !empty($type_info['entity keys']['bundle']) ? '- ' . $entity->{$type_info['entity keys']['bundle']} : '',
      '@title' => entity_label($entity_type, $entity),
    ));
  }
  return $options;
}


/**
 * Implements hook_features_export().
 */
function uuid_entity_features_export($entity_type, $data, &$export, $module_name = '') {
  $pipe = array();
  $container = array();
  $dependences = array();
  $export['dependencies']['uuid_features'] = 'uuid_features';
  $ids = entity_get_id_by_uuid($entity_type, $data);
  foreach ($ids as $id) {
    $container["{$entity_type}_{$id}"] = array('type' => $entity_type, 'id' => $id);
  }
  while (!empty($container)) {
    $container_pop = array_pop($container);
    $entitys = entity_load($container_pop['type'], array($container_pop['id']));
    $entity = current($entitys);
    $dependence = field_entity_dependencies($entity, $container_pop['type']);
    foreach ($dependence as $piece) {
      if (!empty($dependences["{$piece['type']}_{$piece['id']}"])) {
        continue;
      }
      $container["{$piece['type']}_{$piece['id']}"] = $piece;
      $dependences["{$piece['type']}_{$piece['id']}"] = $piece;
      if ($piece['type'] == 'taxonomy_vocabulary') {
        $vocabulary = taxonomy_vocabulary_load($piece['id']);
        $export['features']['taxonomy'][$vocabulary->machine_name] = $vocabulary->machine_name;
      }
      else {
        $uuids = entity_get_uuid_by_id($piece['type'], array($piece['id']));
        $uuid = current($uuids);
        $export['features']["uuid_{$piece['type']}"][$uuid] = $uuid;
      }
    }
  }
  return $pipe;
}

/**
 * Implements hook_features_export_render().
 */
function uuid_entity_features_export_render($entity_type, $module, $data) {
  $code = array();

  uuid_features_load_module_includes();

  $code[] = "  \${$entity_type}s = array();";
  $code[] = '';
  $ids = entity_get_id_by_uuid($entity_type, $data);
  $entities = entity_uuid_load($entity_type, array_keys($ids), NULL, TRUE);
  foreach ($ids as $uuid => $id) {
    // Only export the node if it exists.
    if ($id === FALSE || !isset($entities[$id])) {
      continue;
    }
    // Attempt to load the node, using a fresh cache.

    $entity = $entities[$id];
    $export = clone $entity;

    $black_list = module_invoke_all('uuid_features_entity_render_blacklist');
    if(isset($black_list[$entity_type])) {
      foreach ($black_list[$entity_type] as $list) {
        unset($export->$list);
      }
    }

    // The hook_alter signature is:
    // hook_uuid_node_features_export_render_alter(&$export, $node, $module);
    drupal_alter("uuid_{$entity_type}_features_export_render", $export, $entity, $module);

    $data = array(
      'module' => $module,
      'bundle' => $entity_type
    );
    drupal_alter('uuid_features_export_render', $export, $entity, $data);

    if ($entity_type == 'user' && isset($export->picture)) {
      $export->picture->fid = entity_get_uuid_by_id($entity_type, $export->picture->fid);
      if (($export->picture->uid) != 0 && ($export->picture->uid) != 1) {
        $export->picture->uid = entity_get_uuid_by_id($entity_type, $export->picture->uid);
      }
    }
    $code[] = "  \${$entity_type}s[] = " . features_var_export($export) . ';';
  }

  $code[] = "  return \${$entity_type}s;";
  $code = implode("\n", $code);
  return array("uuid_features_default_$entity_type" => $code);
}

/**
 * Implements hook_uuid_features_entity_render_blacklist()
 */
function uuid_features_uuid_features_entity_render_blacklist() {
  return array(
    'node' => array('nid', 'vid', 'revision_uid', 'revision_timestamp'),
    'file' => array('uid', 'fid', 'timestamp', 'filesize'),
    'user' => array('uid', 'created', 'login', 'access', 'data'),
    'taxonomy_term' => array('vid', 'tid', 'vocabulary_machine_name')
  );
}