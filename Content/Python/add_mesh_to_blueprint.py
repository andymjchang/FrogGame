import unreal

def spawn_mesh(blueprint_path, mesh_path, location_offset=unreal.Vector(0, 0, 0)):
    """
    Adds a Static Mesh Component to the specified Blueprint Asset at a specific location.
    
    :param blueprint_path: String path to the Blueprint (e.g., "/Game/Blueprints/BP_MyCharacter")
    :param mesh_path: String path to the Static Mesh (e.g., "/Game/Props/SM_Wall")
    :param location_offset: unreal.Vector indicating where to place the mesh relative to the root.
    """
    
    # 1. Load the Assets
    bp_asset = unreal.load_asset(blueprint_path)
    mesh_asset = unreal.load_asset(mesh_path)
    
    # Validation
    if not isinstance(bp_asset, unreal.Blueprint):
        unreal.log_error(f"Could not load Blueprint at: {blueprint_path}")
        return
    if not isinstance(mesh_asset, unreal.StaticMesh):
        unreal.log_error(f"Could not load Static Mesh at: {mesh_path}")
        return
    
    # 2. Get the SubobjectDataSubsystem
    subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
    
    # 3. Get subobject handles for the blueprint
    subobject_handles = subsystem.k2_gather_subobject_data_for_blueprint(bp_asset)
    
    if not subobject_handles:
        unreal.log_error("No subobjects found in blueprint")
        return
    
    # 4. Use the first handle as the parent (root)
    root_handle = subobject_handles[0]
    
    # 5. Create new component parameters
    params = unreal.AddNewSubobjectParams()
    params.parent_handle = root_handle
    params.new_class = unreal.StaticMeshComponent
    params.blueprint_context = bp_asset
    
    # 6. Add the new subobject
    new_handle = subsystem.add_new_subobject(params)
    
    if not new_handle:
        unreal.log_error("Failed to create new component")
        return
    
    # 7. Re-gather to get updated list with new component
    updated_handles = subsystem.k2_gather_subobject_data_for_blueprint(bp_asset)
    
    # 8. Find and configure the new component
    # The new component should be in the updated list
    for handle in updated_handles:
        # Try to get the data for this handle
        try:
            data = subsystem.k2_find_subobject_data_from_handle(handle)
            if data:
                component = data.get_object()
                
                # Check if this is our newly created component
                if isinstance(component, unreal.StaticMeshComponent) and component.get_editor_property("static_mesh") is None:
                    # Set the Mesh
                    component.set_editor_property("static_mesh", mesh_asset)
                    
                    # Set the Location
                    component.set_editor_property("relative_location", location_offset)
                    
                    unreal.log(f"Successfully configured component at {location_offset}")
                    break
        except:
            continue
    
    # 9. Compile and save
    unreal.BlueprintEditorLibrary.compile_blueprint(bp_asset)
    unreal.EditorAssetLibrary.save_loaded_asset(bp_asset)
    
    unreal.log(f"Added {mesh_asset.get_name()} to {bp_asset.get_name()} at {location_offset}")

# --- EXECUTION EXAMPLE ---
# import add_mesh_to_blueprint
# add_mesh_to_blueprint.spawn_mesh("/Game/Rooms/BP_Testfafasfsdf.BP_Testfafasfsdf", "/Game/Rooms/SM_Wall.SM_Wall", unreal.Vector(0.0, 0.0, 0.0))