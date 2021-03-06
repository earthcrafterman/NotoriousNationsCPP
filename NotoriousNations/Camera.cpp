#include "Camera.hpp"
#include "Globals.hpp"

const std::map<int, std::string> Camera::m_i_s_output_type_strings
{
	{ CAMERA_OUTPUT_VERTICAL, "Vertical" },
	{ CAMERA_OUTPUT_HORIZONTAL, "Horizontal" },
	{ CAMERA_OUTPUT_ZOOM, "Zoom" }
};

const std::map<std::string, int> Camera::m_s_i_output_type_strings
{
	{ "Vertical", CAMERA_OUTPUT_VERTICAL},
	{ "Horizontal", CAMERA_OUTPUT_HORIZONTAL },
	{ "Zoom", CAMERA_OUTPUT_ZOOM }
};

Camera::Camera()
{
	vec3_position = Vector3(0, 0, 1);

	m_i_m_s_i_outputs = std::map<int, std::map<std::string, int>>();

	m_i_m_s_f_sensitivities = std::map<int, std::map<std::string, float>>();
}

Camera::Camera(std::filesystem::path path_roaming_data_path)
{
	this->path_roaming_data_path = path_roaming_data_path;

	vec3_position = Vector3(0, 0, 1);

	m_i_m_s_i_outputs = std::map<int, std::map<std::string, int>>();

	m_i_m_s_f_sensitivities = std::map<int, std::map<std::string, float>>();

	nlohmann::json json_json;

	std::ifstream ifst_ifstream((path_roaming_data_path.string() + "\\JSON\\Controller\\camera.json").c_str());

	json_json = nlohmann::json::parse(ifst_ifstream);

	for (int i = 0; i < Controller::INPUT_TYPE_NUM_INPUT_TYPES; i++)
	{
		if (!json_json.contains(Controller::m_i_s_input_type_strings.at(i)))
		{
			continue;
		}

		for (auto& [name, key] : json_json.at(Controller::m_i_s_input_type_strings.at(i)).items())
		{
			m_i_m_s_i_outputs[i][name] = m_s_i_output_type_strings.at(key.at("output"));
			m_i_m_s_f_sensitivities[i][name] = key.at("sensitivity");
		}
	}
}

Vector3 Camera::vec3_get_position()
{
	return vec3_position;
}

void Camera::update(float f_delta_time)
{
	Controller cntr_controller = Globals::glob_get_globals().cntr_get_controller(0);

	float f_vertical = 0.0f;
	float f_horizontal = 0.0f;
	float f_zoom = 0.0f;

	for (int i = 0; i < Controller::INPUT_TYPE_NUM_INPUT_TYPES; i++)
	{
		if (!m_i_m_s_i_outputs.contains(i))
		{
			continue;
		}

		for (auto& [name, key] : m_i_m_s_i_outputs[i])
		{
			switch (key)
			{
			case CAMERA_OUTPUT_VERTICAL:
				f_vertical += cntr_controller.f_get_input(i, name) * m_i_m_s_f_sensitivities[i][name];
				break;
			case CAMERA_OUTPUT_HORIZONTAL:
				f_horizontal += cntr_controller.f_get_input(i, name) * m_i_m_s_f_sensitivities[i][name];
				break;
			case CAMERA_OUTPUT_ZOOM:
				f_zoom += cntr_controller.f_get_input(i, name) * m_i_m_s_f_sensitivities[i][name];
				break;
			default:
				break;
			}
		}
	}

	vec3_position = Vector3(vec3_position.x + f_horizontal, vec3_position.y + f_vertical, vec3_position.z + f_zoom);

	if (vec3_position.z < -0.9f)
	{
		vec3_position = Vector3(vec3_position.x, vec3_position.y, -0.9f);
	}
}