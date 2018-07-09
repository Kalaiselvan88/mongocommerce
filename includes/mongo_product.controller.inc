<?php

/**
 * @file
 * Provides a central controller for Mongo Product.
 *
 * This file interacts with the module file and the backend Mongo DB.
 */

// Get the actual Mongo DB field names defined in module settings.
global $_mongo_commerce_collection_name;
global $_mongo_commerce_product_id_field;
global $_mongo_commerce_node_id_field;
global $_mongo_commerce_title_field;
global $_mongo_commerce_price_field;
global $_mongo_commerce_sku_field;
global $_mongo_commerce_size_field;
global $_mongo_commerce_color_field;
global $_mongo_commerce_image_field;
global $_mongo_commerce_status_field;
global $_mongo_commerce_capacity_field;
global $_mongo_commerce_bag_size_field;
global $_mongo_commerce_hat_size_field;
global $_mongo_commerce_shoe_size_field;

$_mongo_commerce_collection_name = variable_get('mongo_commerce_collection', 'amazon_data');
$_mongo_commerce_product_id_field = variable_get('mongo_commerce_product_id', 'product_id');
$_mongo_commerce_node_id_field = variable_get('mongo_commerce_node_id', 'node_id');
$_mongo_commerce_title_field = variable_get('mongo_commerce_title', 'name');
$_mongo_commerce_price_field = variable_get('mongo_commerce_price', 'selling_price');
$_mongo_commerce_sku_field = variable_get('mongo_commerce_sku', 'uniq_id');
$_mongo_commerce_size_field = variable_get('mongo_commerce_size', 'size');
$_mongo_commerce_color_field = variable_get('mongo_commerce_color', 'color');
$_mongo_commerce_image_field = variable_get('mongo_commerce_image', 'image');
$_mongo_commerce_status_field = variable_get('mongo_commerce_status', 'status');
$_mongo_commerce_capacity_field = variable_get('mongo_commerce_capacity', 'capacity');
$_mongo_commerce_bag_size_field = variable_get('mongo_commerce_bag_size', 'bag_size');
$_mongo_commerce_hat_size_field = variable_get('mongo_commerce_hat_size', 'hat_size');
$_mongo_commerce_shoe_size_field = variable_get('mongo_commerce_shoe_size', 'shoe_size');

/**
 * MongoCommerceMongoProductEntityController Class
 *
 * @category Class
 * @package  Commerce
 * @author   Kalaiselvan Swamy
 * @license  http://www.gnu.org/copyleft/gpl.html GNU General Public License
 */
class MongoCommerceMongoProductEntityController extends CommerceProductEntityController {

  /**
   * Loads a product.
   *
   * @param array $ids
   *   The array of product id to load.
   * @param array $conditions
   *   An optional conditions array.
   *
   * @return array
   *   An array of product data.
   */
  public function load($ids = array(), $conditions = array()) {
    // Get the actual Mongo DB field names defined in module settings.
    global $_mongo_commerce_collection_name;
    global $_mongo_commerce_node_id_field;

    $result_document = array();
    $nid = 0;
    $product_collection = mongodb_collection($_mongo_commerce_collection_name);
    // If the current page is a Single Product display page then:
    // get the SKU's of that product and return them as array.
    if (arg(0) == 'node' && is_numeric(arg(1))) {
      // Get the nid
      $nid = arg(1);
      $mongo_prods = $product_collection->find(array($_mongo_commerce_node_id_field => $nid));
      foreach ($mongo_prods as $doc) {
        $result_document[] = $doc;
      }
    }
    // If the current page is a Collection products page then:
    // return all the product data under this collection.
    elseif (arg(0) == 'product_category' && is_numeric(arg(1))) {
      // Get the nodes in this collection and then:
      // fetch their corresponding product details from MongoDB.
      // Then return all the product data under this collection.
      $nodes_in_collection = taxonomy_select_nodes(arg(1), FALSE, FALSE);
      foreach ($nodes_in_collection as $node) {
        $mongo_prods = $product_collection->find(array($_mongo_commerce_node_id_field => $node));
        foreach ($mongo_prods as $doc) {
          $result_document[] = $doc;
        }
      }
    }
    return $result_document;
  }

  /**
   * Saves a product.
   *
   * @param object $product
   *   The full product object to save.
   * @param object $transaction
   *   An optional transaction object.
   *
   * @return none
   *   SAVED_NEW or SAVED_UPDATED depending on the operation performed.
   */
  public function save($product, DatabaseTransaction $transaction = NULL) {
    // Get the actual Mongo DB field names defined in module settings.
    global $_mongo_commerce_collection_name;
    global $_mongo_commerce_product_id_field;
    global $_mongo_commerce_node_id_field;
    global $_mongo_commerce_title_field;
    global $_mongo_commerce_price_field;
    global $_mongo_commerce_sku_field;
    global $_mongo_commerce_size_field;
    global $_mongo_commerce_color_field;
    global $_mongo_commerce_image_field;
    global $_mongo_commerce_status_field;
    global $_mongo_commerce_capacity_field;
    global $_mongo_commerce_bag_size_field;
    global $_mongo_commerce_hat_size_field;
    global $_mongo_commerce_shoe_size_field;

    $product_collection = mongodb_collection($_mongo_commerce_collection_name);
    $prod_to_delete = array();
    if (isset($product->actions['ief_add_save']) && $product->actions['ief_add_save'] == 'Create product') {
      $insert_array                                      = array();
      $insert_array[$_mongo_commerce_product_id_field]   = $product->product_id;
      $insert_array[$_mongo_commerce_sku_field]          = $product->sku;
      $insert_array[$_mongo_commerce_title_field]        = $product->title;
      $insert_array[$_mongo_commerce_price_field]        = $product->commerce_price[LANGUAGE_NONE][0]['amount'];
      $insert_array[$_mongo_commerce_status_field]       = $product->status;

      if (property_exists($product, 'field_top_size')) {
        $insert_array[$_mongo_commerce_size_field]       = $product->field_top_size[LANGUAGE_NONE][0]['tid'];
      }

     if (property_exists($product, 'field_color')) {
        $insert_array[$_mongo_commerce_color_field]      = $product->field_color[LANGUAGE_NONE][0]['tid'];
      }

      if (property_exists($product, 'field_storage_capacity')) {
        $insert_array[$_mongo_commerce_capacity_field]   = $product->field_storage_capacity[LANGUAGE_NONE][0]['tid'];
      }

      if (property_exists($product, 'field_bag_size')) {
        $insert_array[$_mongo_commerce_bag_size_field]   = $product->field_bag_size[LANGUAGE_NONE][0]['tid'];
      }

      if (property_exists($product, 'field_hat_size')) {
        $insert_array[$_mongo_commerce_hat_size_field]   = $product->field_hat_size[LANGUAGE_NONE][0]['tid'];
      }

      if (property_exists($product, 'field_shoe_size')) {
        $insert_array[$_mongo_commerce_shoe_size_field]  = $product->field_shoe_size[LANGUAGE_NONE][0]['tid'];
      }

      if (property_exists($product, 'field_images') && (count($product->field_images[LANGUAGE_NONE]) != 0)) {
        // Get the URI from fid while saving and store in MongoDB.
        $insert_image = '';
        foreach ($product->field_images[LANGUAGE_NONE] as $image) {
          $image_details = file_load($image['fid']);
          $insert_image = $insert_image . $image_details->uri . ",";
        }
        $insert_array[$_mongo_commerce_image_field] = $insert_image;
      }

      if (arg(0) == 'node' && is_numeric(arg(1)) && arg(2) == 'edit') {
        $insert_array[$_mongo_commerce_node_id_field] = arg(1);
      }
      $product_collection->insert($insert_array);
    }
    elseif (isset($product->actions['ief_edit_save']) && $product->actions['ief_edit_save'] == 'Update product') {
      $update_array                                = array();
      $update_array[$_mongo_commerce_sku_field]    = $product->sku;
      $update_array[$_mongo_commerce_title_field]  = $product->title;
      $update_array[$_mongo_commerce_price_field]  = $product->commerce_price[LANGUAGE_NONE][0]['amount'];
      $update_array[$_mongo_commerce_status_field] = $product->status;

      if (property_exists($product, 'field_top_size')) {
        $update_array[$_mongo_commerce_size_field] = $product->field_top_size[LANGUAGE_NONE][0]['tid'];
      }

      if (property_exists($product, 'field_color')) {
        $update_array[$_mongo_commerce_color_field] = $product->field_color[LANGUAGE_NONE][0]['tid'];
      }

      if (property_exists($product, 'field_storage_capacity')) {
        $update_array[$_mongo_commerce_capacity_field] = $product->field_storage_capacity[LANGUAGE_NONE][0]['tid'];
      }

      if (property_exists($product, 'field_bag_size')) {
        $update_array[$_mongo_commerce_bag_size_field] = $product->field_bag_size[LANGUAGE_NONE][0]['tid'];
      }

      if (property_exists($product, 'field_hat_size')) {
        $update_array[$_mongo_commerce_hat_size_field] = $product->field_hat_size[LANGUAGE_NONE][0]['tid'];
      }

      if (property_exists($product, 'field_shoe_size')) {
        $update_array[$_mongo_commerce_shoe_size_field] = $product->field_shoe_size[LANGUAGE_NONE][0]['tid'];
      }

      if (property_exists($product, 'field_images')) {
        // Get the URI from fid while saving and store in MongoDB.
        $insert_image = '';
        foreach ($product->field_images[LANGUAGE_NONE] as $image) {
          $image_details = file_load($image['fid']);
          $insert_image = $insert_image . $image_details->uri . ",";
        }
        $update_array[$_mongo_commerce_image_field] = $insert_image;
      }
      $product_collection->update(array($_mongo_commerce_product_id_field => $product->product_id), array('$set' => $update_array));
    }
    elseif (isset($product->delete) && $product->delete == 'Delete') {
      $prod_to_delete = array_diff($product->original->field_product[LANGUAGE_NONE], $product->field_product[LANGUAGE_NONE]);
      foreach ($prod_to_delete as $prod_delete_id) {
        $product_collection->remove(array($_mongo_commerce_product_id_field => $prod_delete_id['product_id']));
      }
    }
  }

  /**
   * Fetches the product details for the given product ID.
   *
   * @param integer $product_id
   *   The ID of product to be fetched
   *
   * @return array
   *   Product details.
   */
  public function fetchProductDetails($product_id) {
    // Get the actual Mongo DB field names defined in module settings.
    global $_mongo_commerce_collection_name;
    global $_mongo_commerce_product_id_field;

    $product_collection = mongodb_collection($_mongo_commerce_collection_name);
    $mongo_prods = $product_collection->find(array($_mongo_commerce_product_id_field => $product_id));
    foreach ($mongo_prods as $doc) {
      $result_document[] = $doc;
    }
    return $result_document;
  }

  /**
   * Updates the corresponding node ID for the newly added product in MongoDB.
   *
   * @param  object $node_data
   *   The full node object.
   *
   * @return none
   *   None.
   */
  public function updateNodeId($node_data) {
    // Get the actual Mongo DB field names defined in module settings.
    global $_mongo_commerce_collection_name;
    global $_mongo_commerce_product_id_field;
    global $_mongo_commerce_node_id_field;

    $product_collection = mongodb_collection($_mongo_commerce_collection_name);
    // This foreach is to loop through each of the products related to the node:
    // and update the node ID for each product in MongoDB.
    foreach ($node_data->field_product[LANGUAGE_NONE] as $product_ids) {
      $product_collection->update(array($_mongo_commerce_product_id_field => $product_ids['product_id']), array('$set' => array($_mongo_commerce_node_id_field => $node_data->nid)));
    }
  }

  /**
   * Updates the corresponding price for the product being edited through Quick Edit in MongoDB.
   *
   * @param integer $product_id
   *   The ID of product to be updated
   * @param array $price_array
   *   The array containing the price to be updated
   *
   * @return none
   *   None.
   */
  public function updateQuickEdit($product_id, $price_array = array()) {
    // Get the actual Mongo DB field names defined in module settings.
    global $_mongo_commerce_collection_name;
    global $_mongo_commerce_product_id_field;
    global $_mongo_commerce_price_field;

    $product_collection = mongodb_collection($_mongo_commerce_collection_name);
    $product_collection->update(array($_mongo_commerce_product_id_field => $product_id), array('$set' => array($_mongo_commerce_price_field => $price_array[LANGUAGE_NONE][0]['amount'])));
  }

  /**
   * Deletes multiple products by ID.
   *
   * @param $product_ids
   *   An array of product IDs to delete.
   * @param $transaction
   *   An optional transaction object.
   *
   * @return
   *   TRUE on success, FALSE otherwise.
   */
  public function delete($product_ids, DatabaseTransaction $transaction = NULL) {
    // Get the actual Mongo DB field names defined in module settings.
    global $_mongo_commerce_collection_name;
    global $_mongo_commerce_product_id_field;

    $product_collection = mongodb_collection($_mongo_commerce_collection_name);
    foreach ($product_ids as $prod_delete_id) {
      $product_collection->remove(array($_mongo_commerce_product_id_field => $prod_delete_id));
    }
  }

  /**
   * Searches products in Mongo DB.
   *
   * @param $search inputs
   *   An array of form submit search inputs.
   *
   * @return
   *   Search results.
   */
  public function searchMongoDb($search_criteria = array()) {
    global $_mongo_commerce_collection_name;
    $product_collection = mongodb_collection($_mongo_commerce_collection_name);
    if ($search_criteria['search_brand']['#value'] != '') {
      $search_results = $product_collection->find(array('facets1' => array('$elemMatch' => array('type' => 'brand', 'val' => $search_criteria['search_brand']['#value']))));
    }

    if ($search_criteria['search_type']['#value'] != '') {
      $search_results = $product_collection->find(array('facets1' => array('$elemMatch' => array('type' => 'type', 'val' => $search_criteria['search_type']['#value']))));
    }

    foreach ($search_results as $res) {
      $search_result[] = $res;
    }
    return $search_result;
  }

}
