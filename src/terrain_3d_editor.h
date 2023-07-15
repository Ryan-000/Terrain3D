//Copyright � 2023 Roope Palmroos, Cory Petkovsek, and Contributors. All rights reserved. See LICENSE.
#ifndef TERRAIN3D_EDITOR_CLASS_H
#define TERRAIN3D_EDITOR_CLASS_H

#ifdef WIN32
#include <windows.h>
#endif

#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/core/math.hpp>

#include "terrain_3d.h"

using namespace godot;

class Terrain3DEditor : public Object {
	GDCLASS(Terrain3DEditor, Object);

public:
	enum Operation {
		ADD,
		SUBTRACT,
		MULTIPLY,
		DIVIDE,
		REPLACE,
		AVERAGE,
		OP_MAX,
	};

	static inline const char *OPNAME[] = {
		"Add",
		"Subtract",
		"Multiply",
		"Divide",
		"Replace",
		"Average",
		"OP_MAX",
	};

	enum Tool {
		HEIGHT,
		TEXTURE,
		COLOR,
		ROUGHNESS,
		REGION,
		TOOL_MAX,
	};

	static inline const char *TOOLNAME[] = {
		"Height",
		"Texture",
		"Color",
		"Roughness",
		"Region",
		"TOOL_MAX",
	};

	struct Brush {
	private:
		Ref<ImageTexture> texture;
		Ref<Image> image;
		Vector2 img_size;
		int size = 0;
		int index = 0;
		float opacity = 0.0;
		float height = 0.0;
		Color color = COLOR_ROUGHNESS;
		float roughness = 0.5;
		float jitter = 0.0;
		float gamma = 1.0;
		bool align_to_view = false;
		bool auto_regions = false;

	public:
		Ref<ImageTexture> get_texture() const { return texture; }
		Ref<Image> get_image() const { return image; }
		Vector2 get_image_size() const { return img_size; }
		void set_data(Dictionary p_data);
		float get_alpha(Vector2i p_position) { return image->get_pixelv(p_position).r; }
		int get_size() const { return size; }
		int get_index() const { return index; }
		float get_opacity() const { return opacity; }
		float get_height() const { return height; }
		Color get_color() const { return color; }
		float get_roughness() const { return roughness; }
		float get_jitter() const { return jitter; }
		float get_gamma() const { return gamma; }
		bool is_aligned_to_view() const { return align_to_view; }
		bool auto_regions_enabled() const { return auto_regions; }
	};

	Tool tool = REGION;
	Operation operation = ADD;
	Vector3 operation_position = Vector3();
	float operation_interval = 0.0f;
	Brush brush;

	Terrain3D *terrain = nullptr;
	Array _undo_set; // 0-2: map 0,1,2, 3: Region offsets, 4: height range

private:
	void _operate_region(Vector3 p_global_position);
	void _operate_map(Vector3 p_global_position, float p_camera_direction);
	bool _is_in_bounds(Vector2i p_position, Vector2i p_max_position);
	Vector2 _get_uv_position(Vector3 p_global_position, int p_region_size);
	Vector2 _rotate_uv(Vector2 p_uv, float p_angle);

protected:
	static void _bind_methods();

public:
	Terrain3DEditor();
	~Terrain3DEditor();

	inline void set_terrain(Terrain3D *p_terrain) { terrain = p_terrain; }
	inline Terrain3D *get_terrain() const { return terrain; }

	void set_brush_data(Dictionary data);
	void set_tool(Tool p_tool) { tool = p_tool; }
	Tool get_tool() const { return tool; }
	void set_operation(Operation p_operation) { operation = p_operation; }
	Operation get_operation() const { return operation; }
	void operate(Vector3 p_global_position, float p_camera_direction, bool p_continuous_operation);

	void setup_undo();
	void store_undo();
	void apply_undo(const Array &p_set);
};

VARIANT_ENUM_CAST(Terrain3DEditor::Operation);
VARIANT_ENUM_CAST(Terrain3DEditor::Tool);

#endif // TERRAIN3D_EDITOR_CLASS_H